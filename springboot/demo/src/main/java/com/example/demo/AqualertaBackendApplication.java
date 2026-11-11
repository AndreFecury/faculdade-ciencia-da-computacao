package com.example.demo;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationEvent;
import org.springframework.context.ApplicationEventPublisher;
import org.springframework.context.event.EventListener;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Aplicação Principal - Plataforma Aqualerta Backend (Protótipo / MVP)
 * Curso de Ciência da Computação - UNIP 2026
 * * Esta classe inicializa o ecossistema Spring Boot e simula o fluxo
 * assíncrono de eventos (Apache Kafka) e processamento espacial (PostGIS).
 */
@SpringBootApplication
public class AqualertaBackendApplication {

    public static void main(String[] args) {
        SpringApplication.run(AqualertaBackendApplication.class, args);
    }
}

// =========================================================================
// CAMADA DE DOMÍNIO (ENTIDADES)
// =========================================================================

enum Severidade {
    BAIXO, MEDIO, ALTO, CRITICO
}

enum StatusAlerta {
    ATIVO, RESOLVIDO, ARQUIVADO
}

/**
 * Entidade que representa uma Região Monitorada na base de dados espacial.
 * No ambiente real de produção, o atributo 'poligonoWkt' seria mapeado
 * utilizando a biblioteca JTS (Java Topology Suite) e persistido como GEOMETRY(Polygon, 4326) no PostGIS.
 */
@Entity
@Table(name = "regioes_monitoradas")
class RegiaoMonitorada {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    private UUID id;

    @Column(nullable = false, columnDefinition = "")
    private String nome;

    private String descricao;

    // Representação textual geoespacial simplificada da área (Well-Known Text - WKT)
    @Column(columnDefinition = "TEXT", nullable = false)
    private String poligonoWkt;

    private double areaEstimadaKm2;
    private double nivelAlertaMetros;

    // Construtores
    public RegiaoMonitorada() {}

    public RegiaoMonitorada(String nome, String descricao, String poligonoWkt, double areaEstimadaKm2, double nivelAlertaMetros) {
        this.nome = nome;
        this.descricao = descricao;
        this.poligonoWkt = poligonoWkt;
        this.areaEstimadaKm2 = areaEstimadaKm2;
        this.nivelAlertaMetros = nivelAlertaMetros;
    }

    // Getters e Setters
    public UUID getId() { return id; }
    public void setId(UUID id) { this.id = id; }
    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }
    public String getDescricao() { return descricao; }
    public void setDescricao(String descricao) { this.descricao = descricao; }
    public String getPoligonoWkt() { return poligonoWkt; }
    public void setPoligonoWkt(String poligonoWkt) { this.poligonoWkt = poligonoWkt; }
    public double getAreaEstimadaKm2() { return areaEstimadaKm2; }
    public void setAreaEstimadaKm2(double areaEstimadaKm2) { this.areaEstimadaKm2 = areaEstimadaKm2; }
    public double getNivelAlertaMetros() { return nivelAlertaMetros; }
    public void setNivelAlertaMetros(double nivelAlertaMetros) { this.nivelAlertaMetros = nivelAlertaMetros; }
}

/**
 * Representa um evento de inundação mapeado após o processamento da imagem do satélite Sentinel-1 SAR.
 */
@Entity
@Table(name = "eventos_inundacao")
class EventoInundacao {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    private UUID id;

    @ManyToOne
    @JoinColumn(name = "regiao_id", nullable = false)
    private RegiaoMonitorada regiao;

    private LocalDateTime detetadoEm;
    private double areaAfetadaKm2;
    
    @Enumerated(EnumType.STRING)
    private Severidade severidade;

    private double nivelDoRioRegistado;
    private String caminhoFicheiroGeoJson; // Referência para o armazenamento Cloudflare R2 / S3

    public EventoInundacao() {}

    public EventoInundacao(RegiaoMonitorada regiao, LocalDateTime detetadoEm, double areaAfetadaKm2, Severidade severidade, double nivelDoRioRegistado, String caminhoFicheiroGeoJson) {
        this.regiao = regiao;
        this.detetadoEm = detetadoEm;
        this.areaAfetadaKm2 = areaAfetadaKm2;
        this.severidade = severidade;
        this.nivelDoRioRegistado = nivelDoRioRegistado;
        this.caminhoFicheiroGeoJson = caminhoFicheiroGeoJson;
    }

    // Getters e Setters
    public UUID getId() { return id; }
    public RegiaoMonitorada getRegiao() { return regiao; }
    public LocalDateTime getDetetadoEm() { return detetadoEm; }
    public double getAreaAfetadaKm2() { return areaAfetadaKm2; }
    public Severidade getSeveridade() { return severidade; }
    public double getNivelDoRioRegistado() { return nivelDoRioRegistado; }
    public String getCaminhoFicheiroGeoJson() { return caminhoFicheiroGeoJson; }
}

/**
 * Entidade que representa os Alertas Ativos distribuídos para a Proteção Civil e utilizadores locais.
 */
@Entity
@Table(name = "alertas")
class Alerta {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    private UUID id;

    private String titulo;
    private String mensagem;

    @Enumerated(EnumType.STRING)
    private Severidade severidade;

    @Enumerated(EnumType.STRING)
    private StatusAlerta status;

    private LocalDateTime emitidoEm;

    @ManyToOne
    @JoinColumn(name = "regiao_id", nullable = false)
    private RegiaoMonitorada regiao;

    public Alerta() {}

    public Alerta(String titulo, String mensagem, Severidade severidade, RegiaoMonitorada regiao) {
        this.titulo = titulo;
        this.mensagem = mensagem;
        this.severidade = severidade;
        this.status = StatusAlerta.ATIVO;
        this.emitidoEm = LocalDateTime.now();
        this.regiao = regiao;
    }

    // Getters e Setters
    public UUID getId() { return id; }
    public String getTitulo() { return titulo; }
    public String getMensagem() { return mensagem; }
    public Severidade getSeveridade() { return severidade; }
    public StatusAlerta getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }
    public LocalDateTime getEmitidoEm() { return emitidoEm; }
    public RegiaoMonitorada getRegiao() { return regiao; }
}

// =========================================================================
// CAMADA DE PERSISTÊNCIA (REPOSITÓRIOS COM SUPORTE SIMULADO POSTGIS)
// =========================================================================

@Repository
interface RegiaoMonitoradaRepository extends JpaRepository<RegiaoMonitorada, UUID> {

    Object findById(UUID regiaoId);

    List<RegiaoMonitorada> findAll();

    void save(RegiaoMonitorada saoRaimundo);}

@Repository
interface EventoInundacaoRepository extends JpaRepository<EventoInundacao, UUID> {
    List<EventoInundacao> findByRegiaoIdOrderByDetetadoEmDesc(UUID regiaoId);
}

@Repository
interface AlertaRepository extends JpaRepository<Alerta, UUID> {
    List<Alerta> findByStatusOrderByEmitidoEmDesc(StatusAlerta status);

    void save(Alerta alertaInicial);
}

// =========================================================================
// SISTEMA DE EVENTOS ASYNC (SIMULAÇÃO DO COORDENADOR DE EVENTOS / APACHE KAFKA)
// =========================================================================

/**
 * Evento interno do Spring utilizado para simular a chegada de uma nova imagem SAR
 * publicada num tópico do barramento Kafka.
 */
class ImagemSatéliteRecebidaEvent extends ApplicationEvent {
    private final String idImagem;
    private final UUID regiaoId;
    private final double leituraRioNegro;

    public ImagemSatéliteRecebidaEvent(Object source, String idImagem, UUID regiaoId, double leituraRioNegro) {
        super(source);
        this.idImagem = idImagem;
        this.regiaoId = regiaoId;
        this.leituraRioNegro = leituraRioNegro;
    }

    public String getIdImagem() { return idImagem; }
    public UUID getRegiaoId() { return regiaoId; }
    public double getLeituraRioNegro() { return leituraRioNegro; }
}

/**
 * Serviço que escuta o barramento de mensagens e atua como o consumidor da fila 'image.received'.
 * Simula de forma programática a análise de inteligência do OpenCV de maneira assíncrona.
 */
@Component
class ConsumidorKafkaSimulado {

    private final EventoInundacaoRepository eventoRepository;
    private final RegiaoMonitoradaRepository regiaoRepository;
    private final AlertaRepository alertaRepository;
    public ConsumidorKafkaSimulado(EventoInundacaoRepository eventoRepository, 
                                   RegiaoMonitoradaRepository regiaoRepository, 
                                   AlertaRepository alertaRepository,
                                   ApplicationEventPublisher publisher) {
        this.eventoRepository = eventoRepository;
        this.regiaoRepository = regiaoRepository;
        this.alertaRepository = alertaRepository;
    }

    @EventListener
    public void processarMensagemFilaKafka(ImagemSatéliteRecebidaEvent evento) {
        System.out.println("[KAFKA CONSUMER] Fila 'image.received' leu nova mensagem. ID Ficheiro: " + evento.getIdImagem());
        
        RegiaoMonitorada regiao = regiaoRepository.findById(evento.getRegiaoId()).orElse(null);
        if (regiao == null) return;

        // Simulação do processamento OpenCV (Thresholding de Otsu + Filtro adaptativo Speckle)
        System.out.println("[OPENCV WORKER] A analisar reflectância radar da banda-C do satélite...");
        
        double nivelDoRio = evento.getLeituraRioNegro();
        double areaCalculadaInundacao = calcularAreaInundadaSimulada(nivelDoRio, regiao.getAreaEstimadaKm2());
        
        Severidade classeSeveridade = determinarSeveridade(nivelDoRio, regiao.getNivelAlertaMetros());

        // Persiste a vetorização resultante na nossa base de dados geoespacial (Simulação do PostGIS)
        EventoInundacao novoEvento = new EventoInundacao(
            regiao,
            LocalDateTime.now(),
            areaCalculadaInundacao,
            classeSeveridade,
            nivelDoRio,
            "r2://aqualerta-prod/detections/" + regiao.getId() + "/" + evento.getIdImagem() + ".geojson"
        );
        eventoRepository.save(novoEvento);
        System.out.println("[POSTGIS DATABASE] Novo polígono espacial registado sob a chave: " + novoEvento.getId());

        // Caso a severidade ultrapasse o limite de segurança, gera-se um alerta crítico automatizado
        if (classeSeveridade == Severidade.ALTO || classeSeveridade == Severidade.CRITICO) {
            String titulo = "Emergência de Cheia: Região " + regiao.getNome();
            String mensagem = String.format("Aviso Urgente: Nível do Rio Negro atingiu %.2fm na área monitorizada. Extensão da mancha SAR: %.2f km².", 
                nivelDoRio, areaCalculadaInundacao);
            
            Alerta alerta = new Alerta(titulo, mensagem, classeSeveridade, regiao);
            alertaRepository.save(alerta);
            
            System.out.println("[SPRING WEB] Alerta disparado via WebSocket com prioridade: " + classeSeveridade);
        }
    }

    private double calcularAreaInundadaSimulada(double cotaRio, double areaBase) {
        if (cotaRio < 25.0) return 0.5;
        return Math.max(1.0, Math.pow(cotaRio - 24.5, 1.8) * (areaBase / 25.0));
    }

    private Severidade determinarSeveridade(double cotaRio, double limiteAlerta) {
        if (cotaRio >= limiteAlerta + 2.0) return Severidade.CRITICO;
        if (cotaRio >= limiteAlerta + 1.0) return Severidade.ALTO;
        if (cotaRio >= limiteAlerta) return Severidade.MEDIO;
        return Severidade.BAIXO;
    }
}

// =========================================================================
// CAMADA DE CONTROLO (CONTROLADORES REST API)
// =========================================================================

/**
 * API Principal exposta para consumo pelo ecrã frontal do utilizador (Leaflet/App Mobile).
 */
@RestController
@RequestMapping("/api/v1")
@CrossOrigin(origins = "*")
class AqualertaController {

    private final RegiaoMonitoradaRepository regiaoRepository;
    private final EventoInundacaoRepository eventoRepository;
    private final AlertaRepository alertaRepository;
    private final ApplicationEventPublisher eventPublisher;

    public AqualertaController(RegiaoMonitoradaRepository regiaoRepository,
                               EventoInundacaoRepository eventoRepository,
                               AlertaRepository alertaRepository,
                               ApplicationEventPublisher eventPublisher) {
        this.regiaoRepository = regiaoRepository;
        this.eventoRepository = eventoRepository;
        this.alertaRepository = alertaRepository;
        this.eventPublisher = eventPublisher;
    }

    @GetMapping("/regions")
    public List<RegiaoMonitorada> obterRegioes() {
        return regiaoRepository.findAll();
    }

    @GetMapping("/alerts/active")
    public List<Alerta> obterAlertasAtivos() {
        return alertaRepository.findByStatusOrderByEmitidoEmDesc(StatusAlerta.ATIVO);
    }

    @GetMapping("/flood-events/{regionId}")
    public List<EventoInundacao> obterHistoricoDeEventos(@PathVariable UUID regionId) {
        return eventoRepository.findByRegiaoIdOrderByDetetadoEmDesc(regionId);
    }

    /**
     * Endpoint simulador que simula uma passagem orbital programada do satélite Sentinel-1.
     * Acciona o ciclo assíncrono interno via barramento de eventos.
     */
    @PostMapping("/simulate/satellite-pass")
    public Map<String, Object> simularPassagemSatélite(@RequestParam double nivelRio) {
        List<RegiaoMonitorada> regioes = regiaoRepository.findAll();
        if (regioes.isEmpty()) {
            return Map.of("erro", "Nenhuma região monitorada registada no sistema.");
        }

        // Escolhe a primeira região ativa registada (ex: Orla de Manaus)
        RegiaoMonitorada regiaoAlvo = regioes.get(0);
        String idFicheiroSAR = "S1A_IW_GRDH_1SDV_" + System.currentTimeMillis();

        // Publicação fictícia do evento no Kafka (Tratada internamente de forma assíncrona)
        eventPublisher.publishEvent(new ImagemSatéliteRecebidaEvent(this, idFicheiroSAR, regiaoAlvo.getId(), nivelRio));

        Map<String, Object> resposta = new HashMap<>();
        ((Alerta) resposta).setStatus("sucesso");
        resposta.put("mensagem", "Sinal SAR recebido e enviado para a fila Kafka de pré-processamento.");
        resposta.put("ficheiroImagemId", idFicheiroSAR);
        resposta.put("regiaoAlvo", regiaoAlvo.getNome());
        resposta.put("leituraRegistadaRio", nivelRio);
        
        return resposta;
    }
}

// =========================================================================
// RUNNER DE CONFIGURAÇÃO & POPULAÇÃO INICIAL
// =========================================================================

/**
 * Cria registos simulados de base na inicialização da aplicação para testes rápidos.
 */
@Component
class BaseDadosSeedRunner implements CommandLineRunner {

    private final RegiaoMonitoradaRepository regiaoRepository;
    private final AlertaRepository alertaRepository;

    public BaseDadosSeedRunner(RegiaoMonitoradaRepository regiaoRepository, AlertaRepository alertaRepository) {
        this.regiaoRepository = regiaoRepository;
        this.alertaRepository = alertaRepository;
    }

    @Override
    public void run(String... args) throws Exception {
        System.out.println("[DATABASE RUNNER] A popular dados padrão de Manaus (AM)...");

        // Região 1: Bacia do São Raimundo, Zona Oeste de Manaus
        RegiaoMonitorada saoRaimundo = new RegiaoMonitorada(
            "Bacia de São Raimundo",
            "Igarapé e canais fluviais vulneráveis com alta concentração de palafitas residenciais.",
            "POLYGON((-3.111 -60.038, -3.116 -60.033, -3.125 -60.030, -3.120 -60.024, -3.112 -60.029, -3.111 -60.038))",
            35.5,
            27.0
        );
        regiaoRepository.save(saoRaimundo);

        // Região 2: Bacia de Educandos, Zona Sul de Manaus
        RegiaoMonitorada educandos = new RegiaoMonitorada(
            "Bacia de Educandos / Quarenta",
            "Área urbana crítica com risco severo de inundação de vias públicas no período de cheia.",
            "POLYGON((-3.141 -60.002, -3.134 -59.993, -3.128 -59.988, -3.125 -59.991, -3.131 -59.998, -3.141 -60.002))",
            28.2,
            28.0
        );
        regiaoRepository.save(educandos);

        // Criação de Alerta ativo simulado inicial para povoar o ecrã
        Alerta alertaInicial = new Alerta(
            "Cota de Alerta Ultrapassada",
            "Rio Negro ultrapassou a cota operacional de segurança no Porto de Manaus.",
            Severidade.MEDIO,
            saoRaimundo
        );
        alertaRepository.save(alertaInicial);

        System.out.println("[DATABASE RUNNER] Dados de simulação inseridos com sucesso no PostGIS em memória.");
    }
}
