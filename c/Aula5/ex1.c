#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <io.h>
#define E_TERMINAL(f) _isatty(_fileno(f))
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#define E_TERMINAL(f) isatty(fileno(f))
#endif

#define ARQUIVO "alunos.txt"
#define TAGLINE "SISTEMA DE CADASTRO DE ALUNOS  ·  CRUD EM C"
#define TAM_NOME 60
#define TAM_ENTRADA 128
#define LARGURA 76
#define LARGURA_LOGO 52
#define NUM_ITENS 7

#define W_ID 4
#define W_NOME 22
#define W_NOTA 6
#define W_SIT 13

#define FG(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m"
#define BG(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m"

#define RESET "\033[0m"
#define NEGRITO "\033[1m"
#define SEM_NEGRITO "\033[22m"
#define ITALICO "\033[3m"

#define C_BRANCO FG(255, 255, 255)
#define C_TEXTO FG(226, 232, 240)
#define C_SUAVE FG(148, 163, 184)
#define C_APAGADO FG(100, 116, 139)
#define C_BORDA FG(71, 85, 105)
#define C_DESTAQUE FG(129, 140, 248)
#define C_OK FG(74, 222, 128)
#define C_AVISO FG(250, 204, 21)
#define C_ERRO FG(248, 113, 113)

#define BG_LINHA_A BG(15, 23, 42)
#define BG_LINHA_B BG(30, 41, 59)
#define BG_CABECALHO BG(55, 48, 163)

enum {
    TECLA_ENTER = 1000,
    TECLA_ESC,
    TECLA_CIMA,
    TECLA_BAIXO,
    TECLA_ESQ,
    TECLA_DIR,
    TECLA_OUTRA
};

typedef struct {
    int r, g, b;
} Cor;

typedef struct {
    int id;
    char nome[TAM_NOME];
    float nota1;
    float nota2;
} Aluno;

typedef struct {
    const char *rotulo;
    const char *descricao;
    Cor cor;
} ItemMenu;

typedef struct {
    double media;
    int maior;
    int menor;
    int contagem[3];
} Resumo;

static Aluno *alunos = NULL;
static int total = 0;
static int capacidade = 0;
static int criterioOrdem = 1;

static int margem = 0;
static int alturaTerminal = 24;
static int animacoes = 0;
static int telaAlternativa = 0;

#ifndef _WIN32
static struct termios terminalOriginal;
static int modoBruto = 0;
#endif

static const char *NOMES_SITUACAO[] = {"Aprovado", "Recuperação", "Reprovado"};
static const Cor CORES_SITUACAO[] = {{74, 222, 128}, {250, 204, 21}, {248, 113, 113}};

static const ItemMenu ITENS[NUM_ITENS] = {
    {"Cadastrar aluno", "Adiciona um novo aluno com nome e duas notas", {74, 222, 128}},
    {"Listar alunos", "Exibe a tabela completa, com ordenação e páginas", {56, 189, 248}},
    {"Buscar aluno", "Procura por ID ou por parte do nome", {34, 211, 238}},
    {"Editar aluno", "Altera o nome e as notas de um aluno existente", {250, 204, 21}},
    {"Excluir aluno", "Remove um aluno do cadastro, com confirmação", {248, 113, 113}},
    {"Estatísticas", "Médias, ranking e distribuição da turma", {232, 121, 249}},
    {"Sair", "Salva tudo e encerra o programa", {148, 163, 184}}
};

static const char *DIAS[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"};

static const char *LOGO[6] = {
    " █████╗ ██╗     ██╗   ██╗███╗   ██╗ ██████╗ ███████╗",
    "██╔══██╗██║     ██║   ██║████╗  ██║██╔═══██╗██╔════╝",
    "███████║██║     ██║   ██║██╔██╗ ██║██║   ██║███████╗",
    "██╔══██║██║     ██║   ██║██║╚██╗██║██║   ██║╚════██║",
    "██║  ██║███████╗╚██████╔╝██║ ╚████║╚██████╔╝███████║",
    "╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝"
};

/* ---------- Texto e cores ---------- */

static int tamanhoUtf8(const char *texto) {
    int n = 0;

    for (; *texto; texto++) {
        if (((unsigned char)*texto & 0xC0) != 0x80) {
            n++;
        }
    }
    return n;
}

static void repetir(const char *simbolo, int vezes) {
    int i;

    for (i = 0; i < vezes; i++) {
        printf("%s", simbolo);
    }
}

static void pintarFrente(Cor c) {
    printf("\033[38;2;%d;%d;%dm", c.r, c.g, c.b);
}

static void pintarFundo(Cor c) {
    printf("\033[48;2;%d;%d;%dm", c.r, c.g, c.b);
}

static void estiloCor(char *destino, size_t tamanho, Cor c, int negrito) {
    snprintf(destino, tamanho, "%s\033[38;2;%d;%d;%dm", negrito ? NEGRITO : "", c.r, c.g, c.b);
}

static Cor misturar(Cor a, Cor b, double t) {
    Cor c;

    c.r = a.r + (int)((b.r - a.r) * t);
    c.g = a.g + (int)((b.g - a.g) * t);
    c.b = a.b + (int)((b.b - a.b) * t);
    return c;
}

static Cor gradiente(double t) {
    static const Cor paradas[] = {{34, 211, 238}, {129, 140, 248}, {232, 121, 249}};

    if (t < 0) {
        t = 0;
    }
    if (t > 1) {
        t = 1;
    }
    if (t < 0.5) {
        return misturar(paradas[0], paradas[1], t * 2);
    }
    return misturar(paradas[1], paradas[2], (t - 0.5) * 2);
}

static Cor corNota(float nota) {
    static const Cor vermelho = {248, 113, 113};
    static const Cor amarelo = {250, 204, 21};
    static const Cor verde = {74, 222, 128};

    if (nota < 0) {
        nota = 0;
    }
    if (nota > 10) {
        nota = 10;
    }
    if (nota < 5) {
        return misturar(vermelho, amarelo, nota / 5.0);
    }
    return misturar(amarelo, verde, (nota - 5) / 5.0);
}

static void imprimirAjustado(const char *texto, int largura) {
    int tamanho = tamanhoUtf8(texto);
    int limite = tamanho > largura ? largura - 1 : tamanho;
    int impressos = 0;
    const char *p = texto;

    while (*p && impressos < limite) {
        putchar(*p++);
        while (((unsigned char)*p & 0xC0) == 0x80) {
            putchar(*p++);
        }
        impressos++;
    }
    if (tamanho > largura) {
        printf("…");
        impressos++;
    }
    for (; impressos < largura; impressos++) {
        putchar(' ');
    }
}

static void imprimirCentro(const char *texto, int largura) {
    int tamanho = tamanhoUtf8(texto);
    int esquerda, i;

    if (tamanho >= largura) {
        imprimirAjustado(texto, largura);
        return;
    }
    esquerda = (largura - tamanho) / 2;
    for (i = 0; i < esquerda; i++) {
        putchar(' ');
    }
    printf("%s", texto);
    for (i = esquerda + tamanho; i < largura; i++) {
        putchar(' ');
    }
}

static void imprimirGradiente(const char *texto, int inicio, int total_) {
    int posicao = 0;
    const char *p = texto;

    while (*p) {
        if (*p != ' ') {
            pintarFrente(gradiente((double)(inicio + posicao) / (total_ > 1 ? total_ - 1 : 1)));
        }
        putchar(*p++);
        while (((unsigned char)*p & 0xC0) == 0x80) {
            putchar(*p++);
        }
        posicao++;
    }
}

/* ---------- Terminal ---------- */

static void medirTerminal(void) {
    int colunas = 80;
    int linhas = 24;
    const char *ambienteColunas;
    const char *ambienteLinhas;

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        colunas = info.srWindow.Right - info.srWindow.Left + 1;
        linhas = info.srWindow.Bottom - info.srWindow.Top + 1;
    }
#else
    struct winsize tamanho;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &tamanho) == 0 && tamanho.ws_col > 0) {
        colunas = tamanho.ws_col;
        linhas = tamanho.ws_row;
    } else {
        ambienteColunas = getenv("COLUMNS");
        ambienteLinhas = getenv("LINES");
        if (ambienteColunas && atoi(ambienteColunas) > 0) {
            colunas = atoi(ambienteColunas);
        }
        if (ambienteLinhas && atoi(ambienteLinhas) > 0) {
            linhas = atoi(ambienteLinhas);
        }
    }
#endif
    (void)ambienteColunas;
    (void)ambienteLinhas;
    margem = colunas > LARGURA ? (colunas - LARGURA) / 2 : 0;
    alturaTerminal = linhas;
}

static void recuo(void) {
    printf("%*s", margem, "");
}

static void pausaMs(int ms) {
    fflush(stdout);
    if (!animacoes) {
        return;
    }
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    {
        struct timespec t;

        t.tv_sec = ms / 1000;
        t.tv_nsec = (long)(ms % 1000) * 1000000L;
        nanosleep(&t, NULL);
    }
#endif
}

static void mostrarCursor(int visivel) {
    printf(visivel ? "\033[?25h" : "\033[?25l");
}

static void restaurarTerminal(void) {
    if (!telaAlternativa) {
        return;
    }
    telaAlternativa = 0;
    printf(RESET "\033[?25h\033[?1049l");
    fflush(stdout);
}

static void tratarInterrupcao(int sinal) {
    (void)sinal;
#ifndef _WIN32
    if (modoBruto) {
        tcsetattr(STDIN_FILENO, TCSANOW, &terminalOriginal);
    }
#endif
    exit(130);
}

static void iniciarTerminal(void) {
#ifdef _WIN32
    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (saida != INVALID_HANDLE_VALUE && GetConsoleMode(saida, &modo)) {
        SetConsoleMode(saida, modo | 0x0004);
    }
#endif
    animacoes = E_TERMINAL(stdout);
    setvbuf(stdout, NULL, _IOFBF, 1 << 20);
    signal(SIGINT, tratarInterrupcao);
    atexit(restaurarTerminal);

    if (animacoes) {
        printf("\033[?1049h\033]0;Cadastro de Alunos\007");
        telaAlternativa = 1;
    }
}

static void limparTela(void) {
    medirTerminal();
    printf("\033[2J\033[H");
}

/* ---------- Mensagens e blocos visuais ---------- */

static void ln(const char *formato, ...) {
    va_list args;

    recuo();
    va_start(args, formato);
    vprintf(formato, args);
    va_end(args);
    printf(RESET "\033[K\n");
}

static void mensagem(const char *cor, const char *icone, const char *formato, ...) {
    va_list args;

    recuo();
    printf("%s" NEGRITO "%s " SEM_NEGRITO, cor, icone);
    va_start(args, formato);
    vprintf(formato, args);
    va_end(args);
    printf(RESET "\n");
}

#define sucesso(...) mensagem(C_OK, "✔", __VA_ARGS__)
#define aviso(...) mensagem(C_AVISO, "▲", __VA_ARGS__)
#define erro(...) (printf("\a"), mensagem(C_ERRO, "✖", __VA_ARGS__))

static void linhaBorda(const char *esquerda, const char *meio, const char *direita, int largura) {
    int i;

    pintarFrente(gradiente(0));
    printf("%s", esquerda);
    for (i = 1; i < largura - 1; i++) {
        pintarFrente(gradiente((double)i / (largura - 1)));
        printf("%s", meio);
    }
    pintarFrente(gradiente(1));
    printf("%s" RESET, direita);
}

static void cabecalho(const char *titulo) {
    int tamanho = tamanhoUtf8(titulo);
    int esquerda = (LARGURA - 2 - tamanho) / 2;
    int direita = LARGURA - 2 - tamanho - esquerda;

    recuo();
    linhaBorda("╭", "─", "╮", LARGURA);
    printf("\033[K\n");

    recuo();
    pintarFrente(gradiente(0));
    printf("│" RESET "%*s" NEGRITO, esquerda, "");
    imprimirGradiente(titulo, esquerda + 1, LARGURA);
    printf(RESET "%*s", direita, "");
    pintarFrente(gradiente(1));
    printf("│" RESET "\033[K\n");

    recuo();
    linhaBorda("╰", "─", "╯", LARGURA);
    printf("\033[K\n");
}

static void divisor(void) {
    recuo();
    linhaBorda("─", "─", "─", LARGURA);
    printf("\033[K\n");
}

static void desenharBanner(int atrasoMs) {
    int i;

    for (i = 0; i < 6; i++) {
        recuo();
        printf("%*s" NEGRITO, (LARGURA - LARGURA_LOGO) / 2, "");
        imprimirGradiente(LOGO[i], i * 4, LARGURA_LOGO + 24);
        printf(RESET "\033[K\n");
        pausaMs(atrasoMs);
    }
}

static void digitar(const char *texto, int ms) {
    const char *p = texto;

    while (*p) {
        putchar(*p++);
        while (((unsigned char)*p & 0xC0) == 0x80) {
            putchar(*p++);
        }
        pausaMs(ms);
    }
}

static void girar(const char *texto, int quadros) {
    static const char *frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int i;

    if (!animacoes) {
        return;
    }
    for (i = 0; i < quadros; i++) {
        printf("\r");
        recuo();
        pintarFrente(gradiente((double)i / quadros));
        printf("%s " C_SUAVE "%s" RESET "\033[K", frames[i % 10], texto);
        pausaMs(45);
    }
    printf("\r\033[K");
}

static void telaAbertura(void) {
    const int passos = 30;
    const int preenchimento = 34;
    int i, j;

    if (!animacoes) {
        return;
    }
    limparTela();
    mostrarCursor(0);
    for (i = 0; i < (alturaTerminal - 14) / 2; i++) {
        printf("\n");
    }
    desenharBanner(50);
    printf("\n");
    recuo();
    printf("%*s" C_SUAVE, (LARGURA - tamanhoUtf8(TAGLINE)) / 2, "");
    digitar(TAGLINE, 8);
    printf(RESET "\n\n");

    for (i = 0; i <= passos; i++) {
        int cheios = i * preenchimento / passos;

        printf("\r");
        recuo();
        printf("%*s", (LARGURA - preenchimento - 5) / 2, "");
        for (j = 0; j < preenchimento; j++) {
            if (j < cheios) {
                pintarFrente(gradiente((double)j / (preenchimento - 1)));
                printf("█");
            } else {
                printf(C_BORDA "░");
            }
        }
        printf(RESET C_SUAVE " %3d%%" RESET, i * 100 / passos);
        pausaMs(14);
    }
    pausaMs(200);
}

static void despedida(void) {
    const char *texto = "  ●  Até logo! Seus dados estão salvos em " ARQUIVO ".";

    restaurarTerminal();
    printf("\n" NEGRITO);
    imprimirGradiente(texto, 0, tamanhoUtf8(texto));
    printf(RESET "\n\n");
}

/* ---------- Entrada de dados ---------- */

static int lerTeclaDeLinha(void) {
    char linha[TAM_ENTRADA];

    if (fgets(linha, sizeof linha, stdin) == NULL) {
        exit(0);
    }
    if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == '\0') {
        return TECLA_ENTER;
    }
    return (unsigned char)linha[0];
}

static int lerTecla(void) {
    mostrarCursor(0);
    fflush(stdout);

    if (!E_TERMINAL(stdin)) {
        return lerTeclaDeLinha();
    }

#ifdef _WIN32
    {
        int c = _getch();

        if (c == 0 || c == 0xE0) {
            c = _getch();
            switch (c) {
                case 72: return TECLA_CIMA;
                case 80: return TECLA_BAIXO;
                case 75: return TECLA_ESQ;
                case 77: return TECLA_DIR;
                default: return TECLA_OUTRA;
            }
        }
        if (c == 13) {
            return TECLA_ENTER;
        }
        if (c == 27) {
            return TECLA_ESC;
        }
        if (c == 3) {
            exit(130);
        }
        return c;
    }
#else
    {
        struct termios bruto;
        unsigned char c, sequencia[2];
        int tecla;

        tcgetattr(STDIN_FILENO, &terminalOriginal);
        bruto = terminalOriginal;
        bruto.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
        bruto.c_cc[VMIN] = 1;
        bruto.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &bruto);
        modoBruto = 1;

        if (read(STDIN_FILENO, &c, 1) != 1) {
            tcsetattr(STDIN_FILENO, TCSANOW, &terminalOriginal);
            exit(0);
        }

        if (c == 27) {
            bruto.c_cc[VMIN] = 0;
            bruto.c_cc[VTIME] = 1;
            tcsetattr(STDIN_FILENO, TCSANOW, &bruto);
            if (read(STDIN_FILENO, &sequencia[0], 1) == 1 &&
                read(STDIN_FILENO, &sequencia[1], 1) == 1 &&
                (sequencia[0] == '[' || sequencia[0] == 'O')) {
                switch (sequencia[1]) {
                    case 'A': tecla = TECLA_CIMA; break;
                    case 'B': tecla = TECLA_BAIXO; break;
                    case 'C': tecla = TECLA_DIR; break;
                    case 'D': tecla = TECLA_ESQ; break;
                    default: tecla = TECLA_OUTRA;
                }
            } else {
                tecla = TECLA_ESC;
            }
        } else if (c == '\n' || c == '\r') {
            tecla = TECLA_ENTER;
        } else {
            tecla = c;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &terminalOriginal);
        modoBruto = 0;
        return tecla;
    }
#endif
}

static void aparar(char *texto) {
    char *inicio = texto;
    size_t tamanho;

    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    tamanho = strlen(inicio);
    while (tamanho > 0 && isspace((unsigned char)inicio[tamanho - 1])) {
        tamanho--;
    }
    memmove(texto, inicio, tamanho);
    texto[tamanho] = '\0';
}

static void lerCampo(const char *rotulo, char *destino, size_t tamanho) {
    int largura = tamanhoUtf8(rotulo);
    char *quebra;

    if (largura > LARGURA - 8) {
        largura = LARGURA - 8;
    }

    recuo();
    printf(C_BORDA "╭─ " RESET C_DESTAQUE NEGRITO);
    imprimirAjustado(rotulo, largura);
    printf(RESET C_BORDA " ");
    repetir("─", LARGURA - 5 - largura);
    printf("╮" RESET "\n");

    recuo();
    printf(C_BORDA "│ " C_DESTAQUE "► " C_BORDA "%*s│" RESET "\n", LARGURA - 5, "");

    recuo();
    printf(C_BORDA "╰");
    repetir("─", LARGURA - 2);
    printf("╯" RESET "\n");

    printf("\033[2A\033[%dG", margem + 5);
    printf(C_BRANCO);
    mostrarCursor(1);
    fflush(stdout);

    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        printf("\n");
        exit(0);
    }
    printf(RESET "\n");
    mostrarCursor(0);

    quebra = strpbrk(destino, "\r\n");
    if (quebra) {
        *quebra = '\0';
    } else {
        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
    aparar(destino);
}

static int lerInteiro(const char *rotulo) {
    char entrada[TAM_ENTRADA];
    char *fim;
    long valor;

    lerCampo(rotulo, entrada, sizeof entrada);
    valor = strtol(entrada, &fim, 10);
    if (entrada[0] == '\0' || *fim != '\0' || valor < 0 || valor > 1000000) {
        return -1;
    }
    return (int)valor;
}

static void lerNome(const char *rotulo, char *destino, const char *atual) {
    char *p;

    for (;;) {
        lerCampo(rotulo, destino, TAM_NOME);
        if (destino[0] == '\0') {
            if (atual) {
                strcpy(destino, atual);
                return;
            }
            erro("O nome não pode ficar vazio.");
            continue;
        }
        for (p = destino; *p; p++) {
            if (*p == ';') {
                *p = ' ';
            }
        }
        return;
    }
}

static float lerNota(const char *rotulo, const float *atual) {
    char entrada[TAM_ENTRADA];
    char *fim;
    char *p;
    float valor;

    for (;;) {
        lerCampo(rotulo, entrada, sizeof entrada);
        if (entrada[0] == '\0' && atual) {
            return *atual;
        }
        for (p = entrada; *p; p++) {
            if (*p == ',') {
                *p = '.';
            }
        }
        valor = strtof(entrada, &fim);
        if (entrada[0] != '\0' && *fim == '\0' && valor >= 0 && valor <= 10) {
            return valor;
        }
        erro("Nota inválida. Digite um valor entre 0 e 10.");
    }
}

static int confirmar(const char *pergunta) {
    int tecla;

    ln("");
    recuo();
    printf(C_AVISO NEGRITO "? " RESET C_TEXTO "%s " C_APAGADO "[" C_OK "S" C_APAGADO "/" C_ERRO "N"
           C_APAGADO "] " RESET, pergunta);
    tecla = lerTecla();
    if (tecla == 's' || tecla == 'S') {
        printf(C_OK NEGRITO "Sim" RESET "\n");
        return 1;
    }
    printf(C_ERRO NEGRITO "Não" RESET "\n");
    return 0;
}

static void pausar(void) {
    ln("");
    recuo();
    printf(C_APAGADO "Pressione qualquer tecla para continuar..." RESET);
    lerTecla();
}

/* ---------- Regras e dados ---------- */

static float calcularMedia(const Aluno *a) {
    float media = (a->nota1 + a->nota2) / 2;

    return (int)(media * 100 + 0.5f) / 100.0f;
}

static int indiceSituacao(float media) {
    if (media >= 7.0f) {
        return 0;
    }
    if (media >= 5.0f) {
        return 1;
    }
    return 2;
}

static int adicionar(Aluno aluno) {
    if (total == capacidade) {
        int novaCapacidade = capacidade ? capacidade * 2 : 16;
        Aluno *novo = realloc(alunos, (size_t)novaCapacidade * sizeof *novo);

        if (novo == NULL) {
            return 0;
        }
        alunos = novo;
        capacidade = novaCapacidade;
    }
    alunos[total++] = aluno;
    return 1;
}

static int proximoId(void) {
    int maior = 0;
    int i;

    for (i = 0; i < total; i++) {
        if (alunos[i].id > maior) {
            maior = alunos[i].id;
        }
    }
    return maior + 1;
}

static int buscarIndicePorId(int id) {
    int i;

    for (i = 0; i < total; i++) {
        if (alunos[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int contemTexto(const char *texto, const char *busca) {
    int i, j;

    for (i = 0; texto[i] != '\0'; i++) {
        for (j = 0; busca[j] != '\0' &&
                    tolower((unsigned char)texto[i + j]) == tolower((unsigned char)busca[j]); j++) {
        }
        if (busca[j] == '\0') {
            return 1;
        }
    }
    return 0;
}

static int compararTexto(const char *a, const char *b) {
    while (*a && tolower((unsigned char)*a) == tolower((unsigned char)*b)) {
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

static Resumo resumirTurma(void) {
    Resumo r = {0, 0, 0, {0, 0, 0}};
    double soma = 0;
    int i;

    for (i = 0; i < total; i++) {
        float media = calcularMedia(&alunos[i]);

        soma += media;
        r.contagem[indiceSituacao(media)]++;
        if (media > calcularMedia(&alunos[r.maior])) {
            r.maior = i;
        }
        if (media < calcularMedia(&alunos[r.menor])) {
            r.menor = i;
        }
    }
    if (total > 0) {
        r.media = soma / total;
    }
    return r;
}

static void carregar(void) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    char linha[256];

    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof linha, arquivo) != NULL) {
        char *id = strtok(linha, ";");
        char *nome = strtok(NULL, ";");
        char *nota1 = strtok(NULL, ";");
        char *nota2 = strtok(NULL, ";\r\n");
        Aluno aluno;

        if (!id || !nome || !nota1 || !nota2) {
            continue;
        }
        aluno.id = atoi(id);
        snprintf(aluno.nome, sizeof aluno.nome, "%s", nome);
        aluno.nota1 = strtof(nota1, NULL);
        aluno.nota2 = strtof(nota2, NULL);
        adicionar(aluno);
    }
    fclose(arquivo);
}

static int salvar(void) {
    FILE *arquivo = fopen(ARQUIVO, "w");
    int i;

    if (arquivo == NULL) {
        erro("Não foi possível gravar o arquivo %s.", ARQUIVO);
        return 0;
    }
    for (i = 0; i < total; i++) {
        fprintf(arquivo, "%d;%s;%.2f;%.2f\n", alunos[i].id, alunos[i].nome,
                alunos[i].nota1, alunos[i].nota2);
    }
    fclose(arquivo);
    girar("Gravando em " ARQUIVO, 8);
    return 1;
}

/* ---------- Tabela ---------- */

static void bordaTabela(const char *esquerda, const char *meio, const char *direita) {
    int larguras[] = {W_ID, W_NOME, W_NOTA, W_NOTA, W_NOTA, W_SIT};
    int i;

    recuo();
    printf(C_BORDA "%s", esquerda);
    for (i = 0; i < 6; i++) {
        repetir("─", larguras[i] + 2);
        printf("%s", i < 5 ? meio : direita);
    }
    printf(RESET "\n");
}

static void celula(const char *texto, int largura, const char *estilo, const char *fundo, int centro) {
    printf("%s" C_BORDA "│ %s", fundo, estilo);
    if (centro) {
        imprimirCentro(texto, largura);
    } else {
        imprimirAjustado(texto, largura);
    }
    printf(SEM_NEGRITO " ");
}

static void cabecalhoTabela(void) {
    const char *estilo = NEGRITO C_BRANCO;

    bordaTabela("╭", "┬", "╮");
    recuo();
    celula("ID", W_ID, estilo, BG_CABECALHO, 0);
    celula("Nome", W_NOME, estilo, BG_CABECALHO, 0);
    celula("Nota 1", W_NOTA, estilo, BG_CABECALHO, 1);
    celula("Nota 2", W_NOTA, estilo, BG_CABECALHO, 1);
    celula("Média", W_NOTA, estilo, BG_CABECALHO, 1);
    celula("Situação", W_SIT, estilo, BG_CABECALHO, 0);
    printf("%s" C_BORDA "│" RESET "\n", BG_CABECALHO);
    bordaTabela("├", "┼", "┤");
}

static void linhaAluno(const Aluno *a, int zebra) {
    const char *fundo = zebra ? BG_LINHA_B : BG_LINHA_A;
    char texto[32];
    char estilo[48];
    float media = calcularMedia(a);
    int situacao = indiceSituacao(media);

    recuo();
    snprintf(texto, sizeof texto, "%d", a->id);
    celula(texto, W_ID, C_APAGADO, fundo, 0);
    celula(a->nome, W_NOME, C_TEXTO, fundo, 0);

    snprintf(texto, sizeof texto, "%.2f", a->nota1);
    estiloCor(estilo, sizeof estilo, corNota(a->nota1), 0);
    celula(texto, W_NOTA, estilo, fundo, 1);

    snprintf(texto, sizeof texto, "%.2f", a->nota2);
    estiloCor(estilo, sizeof estilo, corNota(a->nota2), 0);
    celula(texto, W_NOTA, estilo, fundo, 1);

    snprintf(texto, sizeof texto, "%.2f", media);
    estiloCor(estilo, sizeof estilo, corNota(media), 1);
    celula(texto, W_NOTA, estilo, fundo, 1);

    snprintf(texto, sizeof texto, "● %s", NOMES_SITUACAO[situacao]);
    estiloCor(estilo, sizeof estilo, CORES_SITUACAO[situacao], 0);
    celula(texto, W_SIT, estilo, fundo, 0);

    printf("%s" C_BORDA "│" RESET "\n", fundo);
}

static void mostrarTabela(const int *indices, int quantidade) {
    int i;

    cabecalhoTabela();
    for (i = 0; i < quantidade; i++) {
        linhaAluno(&alunos[indices[i]], i % 2);
    }
    bordaTabela("╰", "┴", "╯");
}

static int compararAlunos(const void *x, const void *y) {
    const Aluno *a = &alunos[*(const int *)x];
    const Aluno *b = &alunos[*(const int *)y];
    float mediaA, mediaB;

    switch (criterioOrdem) {
        case 2:
            return compararTexto(a->nome, b->nome);
        case 3:
            mediaA = calcularMedia(a);
            mediaB = calcularMedia(b);
            return (mediaB > mediaA) - (mediaB < mediaA);
        default:
            return a->id - b->id;
    }
}

static void visualizarAlunos(const char *titulo, const int *indices, int quantidade) {
    int pagina = 0;

    for (;;) {
        int porPagina, paginas, inicio, fim, tecla;

        limparTela();
        porPagina = alturaTerminal - 14;
        if (porPagina < 3) {
            porPagina = 3;
        }
        if (porPagina > 25) {
            porPagina = 25;
        }
        paginas = (quantidade + porPagina - 1) / porPagina;
        if (paginas < 1) {
            paginas = 1;
        }
        if (pagina >= paginas) {
            pagina = paginas - 1;
        }
        inicio = pagina * porPagina;
        fim = inicio + porPagina < quantidade ? inicio + porPagina : quantidade;

        cabecalho(titulo);
        ln("");
        mostrarTabela(indices + inicio, fim - inicio);
        ln("");
        ln(C_SUAVE "%d aluno(s)  ·  página " C_BRANCO NEGRITO "%d" RESET C_SUAVE " de %d", quantidade,
           pagina + 1, paginas);
        if (paginas > 1) {
            ln(C_APAGADO "← → trocar de página   Q voltar ao menu");
        } else {
            ln(C_APAGADO "Pressione qualquer tecla para voltar ao menu");
        }

        tecla = lerTecla();
        if (tecla == 'q' || tecla == 'Q' || tecla == TECLA_ESC) {
            return;
        }
        if (tecla == TECLA_ESQ || tecla == 'p' || tecla == 'P') {
            if (pagina > 0) {
                pagina--;
            }
        } else if (pagina < paginas - 1) {
            pagina++;
        } else {
            return;
        }
    }
}

/* ---------- Menu ---------- */

static void desenharTagline(void) {
    recuo();
    printf("%*s" C_SUAVE TAGLINE RESET "\033[K\n", (LARGURA - tamanhoUtf8(TAGLINE)) / 2, "");
}

static void desenharCartoes(void) {
    static const char *rotulos[] = {"ALUNOS", "MÉDIA DA TURMA", "APROVAÇÃO"};
    static const Cor cores[] = {{56, 189, 248}, {232, 121, 249}, {74, 222, 128}};
    char valores[3][64];
    Resumo resumo = resumirTurma();
    int porcentagem = total > 0 ? resumo.contagem[0] * 100 / total : 0;
    int cheios = porcentagem * 8 / 100;
    int linha, k, i;

    snprintf(valores[0], sizeof valores[0], "%d cadastrado%s", total, total == 1 ? "" : "s");
    if (total > 0) {
        snprintf(valores[1], sizeof valores[1], "%.2f  de 10", resumo.media);
    } else {
        snprintf(valores[1], sizeof valores[1], "—");
    }
    snprintf(valores[2], sizeof valores[2], "%d%%  ", porcentagem);
    for (i = 0; i < 8; i++) {
        strcat(valores[2], i < cheios ? "█" : "░");
    }

    for (linha = 0; linha < 4; linha++) {
        recuo();
        for (k = 0; k < 3; k++) {
            pintarFrente(cores[k]);
            switch (linha) {
                case 0:
                    printf("╭");
                    repetir("─", 22);
                    printf("╮");
                    break;
                case 1:
                    printf("│ " C_APAGADO);
                    imprimirAjustado(rotulos[k], 20);
                    pintarFrente(cores[k]);
                    printf(" │");
                    break;
                case 2:
                    printf("│ " NEGRITO);
                    imprimirAjustado(valores[k], 20);
                    printf(SEM_NEGRITO " │");
                    break;
                default:
                    printf("╰");
                    repetir("─", 22);
                    printf("╯");
            }
            printf(RESET "%s", k < 2 ? "  " : "");
        }
        printf("\033[K\n");
    }
}

static void desenharItens(int selecionado) {
    int i;

    for (i = 0; i < NUM_ITENS; i++) {
        Cor cor = ITENS[i].cor;
        char numero = i == NUM_ITENS - 1 ? '0' : (char)('1' + i);

        recuo();
        if (i == selecionado) {
            Cor fundo;

            fundo.r = cor.r / 6 + 10;
            fundo.g = cor.g / 6 + 10;
            fundo.b = cor.b / 6 + 10;
            pintarFundo(fundo);
            pintarFrente(cor);
            printf("▌ ► " NEGRITO "%c" SEM_NEGRITO "  ", numero);
            printf(C_BRANCO NEGRITO);
            imprimirAjustado(ITENS[i].rotulo, LARGURA - 7);
        } else {
            printf("    ");
            pintarFrente(cor);
            printf("%c  " C_SUAVE, numero);
            imprimirAjustado(ITENS[i].rotulo, LARGURA - 7);
        }
        printf(RESET "\033[K\n");
    }
}

static void desenharDica(int selecionado) {
    recuo();
    pintarFrente(ITENS[selecionado].cor);
    printf("► " ITALICO C_SUAVE "%s" RESET "\033[K\n", ITENS[selecionado].descricao);
}

static void desenharRodape(void) {
    const char *teclas = "↑ ↓ navegar   Enter selecionar   0-6 atalho   Q sair";
    char dataHora[48] = "";
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    int espaco;

    if (t != NULL) {
        snprintf(dataHora, sizeof dataHora, "%s %02d/%02d/%04d  %02d:%02d", DIAS[t->tm_wday],
                 t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
    }
    espaco = LARGURA - tamanhoUtf8(teclas) - tamanhoUtf8(dataHora);
    if (espaco < 1) {
        espaco = 1;
    }
    recuo();
    printf(C_APAGADO "%s%*s%s" RESET "\033[K\n", teclas, espaco, "", dataHora);
}

static void desenharMenu(int selecionado) {
    medirTerminal();
    printf("\033[H");

    if (alturaTerminal >= 28) {
        ln("");
        desenharBanner(0);
        desenharTagline();
        ln("");
        desenharCartoes();
        ln("");
    } else {
        cabecalho("SISTEMA DE CADASTRO DE ALUNOS");
        ln("");
    }

    desenharItens(selecionado);
    ln("");
    desenharDica(selecionado);
    divisor();
    desenharRodape();
    printf("\033[J");
}

static int menuPrincipal(int *selecionado) {
    for (;;) {
        int tecla;

        desenharMenu(*selecionado);
        tecla = lerTecla();

        switch (tecla) {
            case TECLA_CIMA:
                *selecionado = (*selecionado + NUM_ITENS - 1) % NUM_ITENS;
                break;
            case TECLA_BAIXO:
                *selecionado = (*selecionado + 1) % NUM_ITENS;
                break;
            case TECLA_ENTER:
                return *selecionado == NUM_ITENS - 1 ? 0 : *selecionado + 1;
            case 'q':
            case 'Q':
                return 0;
            default:
                if (tecla >= '0' && tecla <= '6') {
                    *selecionado = tecla == '0' ? NUM_ITENS - 1 : tecla - '1';
                    return tecla - '0';
                }
        }
    }
}

/* ---------- Operações do CRUD ---------- */

static int selecionarAluno(void) {
    int id = lerInteiro("ID do aluno");
    int indice;

    if (id <= 0) {
        erro("ID inválido.");
        return -1;
    }
    indice = buscarIndicePorId(id);
    if (indice < 0) {
        erro("Nenhum aluno encontrado com o ID %d.", id);
    }
    return indice;
}

static void cadastrarAluno(void) {
    do {
        Aluno aluno;
        int indice;

        limparTela();
        cabecalho("CADASTRAR ALUNO");
        aluno.id = proximoId();
        ln("");
        ln(C_APAGADO "Novo registro  " C_DESTAQUE NEGRITO "#%d", aluno.id);
        ln("");
        lerNome("Nome completo", aluno.nome, NULL);
        aluno.nota1 = lerNota("Primeira nota  (0 a 10)", NULL);
        aluno.nota2 = lerNota("Segunda nota  (0 a 10)", NULL);

        limparTela();
        cabecalho("CADASTRAR ALUNO");
        ln("");
        if (!adicionar(aluno)) {
            erro("Memória insuficiente para cadastrar o aluno.");
        } else if (salvar()) {
            sucesso("Aluno cadastrado com sucesso!");
            ln("");
            indice = total - 1;
            mostrarTabela(&indice, 1);
        }
    } while (confirmar("Cadastrar outro aluno?"));
}

static int escolherOrdem(void) {
    int tecla;

    ln(C_SUAVE "Ordenar por:   " C_DESTAQUE NEGRITO "1" RESET C_SUAVE " ID     " C_DESTAQUE NEGRITO "2"
       RESET C_SUAVE " Nome     " C_DESTAQUE NEGRITO "3" RESET C_SUAVE " Maior média");
    recuo();
    printf(C_APAGADO "Pressione 1, 2 ou 3  (Enter = ID)" RESET);
    tecla = lerTecla();
    return (tecla == '2' || tecla == '3') ? tecla - '0' : 1;
}

static void listarAlunos(void) {
    int *indices;
    int i;

    limparTela();
    cabecalho("LISTA DE ALUNOS");
    ln("");

    if (total == 0) {
        aviso("Nenhum aluno cadastrado.");
        pausar();
        return;
    }

    criterioOrdem = escolherOrdem();

    indices = malloc((size_t)total * sizeof *indices);
    if (indices == NULL) {
        erro("Memória insuficiente.");
        pausar();
        return;
    }
    for (i = 0; i < total; i++) {
        indices[i] = i;
    }
    qsort(indices, (size_t)total, sizeof *indices, compararAlunos);
    visualizarAlunos("LISTA DE ALUNOS", indices, total);
    free(indices);
}

static void buscarAluno(void) {
    char termo[TAM_ENTRADA];
    int *indices;
    int encontrados = 0;
    int i;

    limparTela();
    cabecalho("BUSCAR ALUNO");
    ln("");

    if (total == 0) {
        aviso("Nenhum aluno cadastrado.");
        pausar();
        return;
    }

    lerCampo("ID ou parte do nome", termo, sizeof termo);
    if (termo[0] == '\0') {
        aviso("Busca vazia.");
        pausar();
        return;
    }

    indices = malloc((size_t)total * sizeof *indices);
    if (indices == NULL) {
        erro("Memória insuficiente.");
        pausar();
        return;
    }

    if (strspn(termo, "0123456789") == strlen(termo)) {
        int indice = strlen(termo) <= 9 ? buscarIndicePorId(atoi(termo)) : -1;

        if (indice >= 0) {
            indices[encontrados++] = indice;
        }
    } else {
        for (i = 0; i < total; i++) {
            if (contemTexto(alunos[i].nome, termo)) {
                indices[encontrados++] = i;
            }
        }
    }

    if (encontrados == 0) {
        aviso("Nenhum aluno encontrado para \"%s\".", termo);
        pausar();
    } else {
        visualizarAlunos("RESULTADO DA BUSCA", indices, encontrados);
    }
    free(indices);
}

static void editarAluno(void) {
    char rotulo[TAM_ENTRADA];
    char nome[TAM_NOME];
    float nota1, nota2;
    int indice;
    Aluno *aluno;

    limparTela();
    cabecalho("EDITAR ALUNO");
    ln("");

    if (total == 0) {
        aviso("Nenhum aluno cadastrado.");
        pausar();
        return;
    }

    indice = selecionarAluno();
    if (indice < 0) {
        pausar();
        return;
    }
    aluno = &alunos[indice];

    ln("");
    mostrarTabela(&indice, 1);
    ln("");
    ln(C_APAGADO "Pressione ENTER em um campo para manter o valor atual.");
    ln("");

    snprintf(rotulo, sizeof rotulo, "Nome  ·  atual: %s", aluno->nome);
    lerNome(rotulo, nome, aluno->nome);
    snprintf(rotulo, sizeof rotulo, "Primeira nota  ·  atual: %.2f", aluno->nota1);
    nota1 = lerNota(rotulo, &aluno->nota1);
    snprintf(rotulo, sizeof rotulo, "Segunda nota  ·  atual: %.2f", aluno->nota2);
    nota2 = lerNota(rotulo, &aluno->nota2);

    snprintf(aluno->nome, sizeof aluno->nome, "%s", nome);
    aluno->nota1 = nota1;
    aluno->nota2 = nota2;

    limparTela();
    cabecalho("EDITAR ALUNO");
    ln("");
    if (salvar()) {
        sucesso("Aluno atualizado com sucesso!");
        ln("");
        mostrarTabela(&indice, 1);
    }
    pausar();
}

static void excluirAluno(void) {
    int indice;

    limparTela();
    cabecalho("EXCLUIR ALUNO");
    ln("");

    if (total == 0) {
        aviso("Nenhum aluno cadastrado.");
        pausar();
        return;
    }

    indice = selecionarAluno();
    if (indice < 0) {
        pausar();
        return;
    }

    ln("");
    mostrarTabela(&indice, 1);

    if (confirmar("Tem certeza que deseja excluir este aluno?")) {
        memmove(&alunos[indice], &alunos[indice + 1],
                (size_t)(total - indice - 1) * sizeof *alunos);
        total--;
        ln("");
        if (salvar()) {
            sucesso("Aluno excluído com sucesso!");
        }
    } else {
        ln("");
        aviso("Operação cancelada.");
    }
    pausar();
}

static void barraSimples(int cheios, int largura, Cor cor) {
    int i;

    for (i = 0; i < largura; i++) {
        if (i < cheios) {
            pintarFrente(cor);
            printf("█");
        } else {
            printf(C_BORDA "░");
        }
    }
    printf(RESET);
}

static void barraNota(float media, int largura) {
    int cheios = (int)(media / 10.0f * largura + 0.5f);
    int i;

    for (i = 0; i < largura; i++) {
        if (i < cheios) {
            pintarFrente(corNota(10.0f * i / largura));
            printf("█");
        } else {
            printf(C_BORDA "░");
        }
    }
    printf(RESET);
}

static void mostrarEstatisticas(void) {
    static const char *NIVEIS[] = {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"};
    static const Cor MEDALHAS[] = {{250, 204, 21}, {203, 213, 225}, {251, 146, 60}};
    Resumo resumo;
    float mediaMaior, mediaMenor;
    int *indices;
    int ranking, i;

    limparTela();
    cabecalho("ESTATÍSTICAS DA TURMA");
    ln("");

    if (total == 0) {
        aviso("Nenhum aluno cadastrado.");
        pausar();
        return;
    }

    resumo = resumirTurma();
    mediaMaior = calcularMedia(&alunos[resumo.maior]);
    mediaMenor = calcularMedia(&alunos[resumo.menor]);

    recuo();
    printf(C_SUAVE "Total de alunos   " C_BRANCO NEGRITO "%d" RESET "\n", total);
    recuo();
    printf(C_SUAVE "Média da turma    ");
    pintarFrente(corNota((float)resumo.media));
    printf(NEGRITO "%.2f" RESET "\n", resumo.media);
    recuo();
    printf(C_SUAVE "Maior média       ");
    pintarFrente(corNota(mediaMaior));
    printf(NEGRITO "%.2f" RESET C_APAGADO "  %s" RESET "\n", mediaMaior, alunos[resumo.maior].nome);
    recuo();
    printf(C_SUAVE "Menor média       ");
    pintarFrente(corNota(mediaMenor));
    printf(NEGRITO "%.2f" RESET C_APAGADO "  %s" RESET "\n", mediaMenor, alunos[resumo.menor].nome);

    ln("");
    recuo();
    printf(C_SUAVE "Médias por aluno  ");
    for (i = 0; i < total && i < LARGURA - 20; i++) {
        float media = calcularMedia(&alunos[i]);
        int nivel = (int)(media / 10.0f * 8);

        if (nivel > 7) {
            nivel = 7;
        }
        pintarFrente(corNota(media));
        printf("%s", NIVEIS[nivel]);
    }
    printf(RESET "\n");

    ln("");
    for (i = 0; i < 3; i++) {
        recuo();
        pintarFrente(CORES_SITUACAO[i]);
        printf("● " C_TEXTO);
        imprimirAjustado(NOMES_SITUACAO[i], W_SIT);
        printf(" ");
        barraSimples(resumo.contagem[i] * 24 / total, 24, CORES_SITUACAO[i]);
        printf("  " NEGRITO "%d" RESET C_APAGADO " (%d%%)" RESET "\n", resumo.contagem[i],
               resumo.contagem[i] * 100 / total);
    }

    ranking = alturaTerminal - 24;
    if (ranking > 5) {
        ranking = 5;
    }
    if (ranking > total) {
        ranking = total;
    }
    if (ranking > 0) {
        indices = malloc((size_t)total * sizeof *indices);
        if (indices != NULL) {
            for (i = 0; i < total; i++) {
                indices[i] = i;
            }
            criterioOrdem = 3;
            qsort(indices, (size_t)total, sizeof *indices, compararAlunos);

            ln("");
            ln(C_DESTAQUE NEGRITO "RANKING");
            for (i = 0; i < ranking; i++) {
                const Aluno *a = &alunos[indices[i]];
                float media = calcularMedia(a);

                recuo();
                pintarFrente(i < 3 ? MEDALHAS[i] : corNota(5));
                printf(NEGRITO "%d" RESET "  " C_TEXTO, i + 1);
                imprimirAjustado(a->nome, 24);
                printf(" ");
                barraNota(media, 24);
                pintarFrente(corNota(media));
                printf("  " NEGRITO "%.2f" RESET "\n", media);
            }
            free(indices);
        }
    }
    pausar();
}

int main(void) {
    int selecionado = 0;
    int opcao;

    iniciarTerminal();
    carregar();
    telaAbertura();

    do {
        limparTela();
        opcao = menuPrincipal(&selecionado);

        switch (opcao) {
            case 1:
                cadastrarAluno();
                break;
            case 2:

            
                listarAlunos();
                break;
            case 3:
                buscarAluno();
                break;
            case 4:
                editarAluno();
                break;
            case 5:
                excluirAluno();
                break;
            case 6:
                mostrarEstatisticas();
                break;
            default:
                break;
        }
    } while (opcao != 0);

    despedida();
    free(alunos);
    return 0;
}