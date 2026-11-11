#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_CONTAS 10
#define LARGURA 40

/* ===== CORES ANSI ===== */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

struct Conta {
    int numero;
    int senha;
    float saldo;
    char nome[30];
    char extrato[10][60];
    int qtdMovimentos;
};

/* ===== FUNÇÕES DE VISUAL (ASCII puro, sempre alinha) ===== */

void limparTela(void) {
    system("cls");
}

void pausar(void) {
    printf(YELLOW "\nPressione ENTER para continuar..." RESET);
    getchar();
}

void borda(void) {
    printf(CYAN "+");
    for (int i = 0; i < LARGURA; i++) printf("-");
    printf("+\n" RESET);
}

void tituloCentralizado(const char *texto) {
    int tam = (int)strlen(texto);
    int espacoTotal = LARGURA - tam;
    int esq = espacoTotal / 2;
    int dir = espacoTotal - esq;
    if (esq < 0) esq = 0;
    if (dir < 0) dir = 0;

    printf(CYAN "|" RESET);
    for (int i = 0; i < esq; i++) printf(" ");
    printf(BOLD "%s" RESET, texto);
    for (int i = 0; i < dir; i++) printf(" ");
    printf(CYAN "|\n" RESET);
}

void linhaTexto(const char *texto) {
    printf(CYAN "| " RESET "%-*s" CYAN "|\n" RESET, LARGURA - 1, texto);
}

void opcaoMenu(const char *numero, const char *texto) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%s %s", numero, texto);
    linhaTexto(buffer);
}

void topo(const char *titulo) {
    borda();
    tituloCentralizado(titulo);
    borda();
}

void sucesso(const char *msg) {
    printf(GREEN "\n[OK] %s\n" RESET, msg);
}

void erro(const char *msg) {
    printf(RED "\n[ERRO] %s\n" RESET, msg);
}

void aviso(const char *msg) {
    printf(YELLOW "\n[AVISO] %s\n" RESET, msg);
}

/* ===== FUNÇÕES DO SISTEMA ===== */

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void mostrarExtrato(struct Conta *conta) {
    topo("EXTRATO");
    if (conta->qtdMovimentos == 0) {
        linhaTexto("Nenhuma movimentacao registrada.");
    } else {
        for (int i = 0; i < conta->qtdMovimentos; i++) {
            linhaTexto(conta->extrato[i]);
        }
    }
    borda();
}

void adicionarMovimento(struct Conta *conta, const char *texto) {
    if (conta->qtdMovimentos < 10) {
        snprintf(conta->extrato[conta->qtdMovimentos], 60, "%s", texto);
        conta->qtdMovimentos++;
    }
}

int contaExiste(struct Conta contas[], int qtd, int numero) {
    for (int i = 0; i < qtd; i++) {
        if (contas[i].numero == numero) return 1;
    }
    return 0;
}

void criarConta(struct Conta contas[], int *qtdContas) {
    if (*qtdContas >= MAX_CONTAS) {
        erro("Limite de contas atingido.");
        pausar();
        return;
    }

    struct Conta nova = {0};

    topo("CRIAR NOVA CONTA");

    printf(CYAN "| " RESET "Seu nome: ");
    scanf(" %29[^\n]", nova.nome);
    limparBuffer();

    int numero;
    do {
        printf(CYAN "| " RESET "Escolha o numero da conta: ");
        scanf("%d", &numero);
        limparBuffer();

        if (contaExiste(contas, *qtdContas, numero)) {
            aviso("Esse numero de conta ja existe. Escolha outro.");
        }
    } while (contaExiste(contas, *qtdContas, numero));

    int senha;
    printf(CYAN "| " RESET "Escolha uma senha (4 digitos): ");
    scanf("%d", &senha);
    limparBuffer();

    nova.numero = numero;
    nova.senha = senha;
    nova.saldo = 0.0f;
    nova.qtdMovimentos = 0;

    contas[*qtdContas] = nova;
    (*qtdContas)++;

    borda();
    sucesso("CONTA CRIADA COM SUCESSO!");
    printf("Numero da conta: %d\n", numero);
    pausar();
}

int main(void) {
    /* Ativa cores ANSI no console do Windows */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    struct Conta contas[MAX_CONTAS] = {0};
    int qtdContas = 0;

    int opcao, opcaoConta, indiceLogin = -1;
    int contaDigitada, senhaDigitada;
    float valor;
    char movimento[60];

    do {
        limparTela();
        topo("BANCO DO POVO");
        opcaoMenu("1 -", "CRIAR CONTA");
        opcaoMenu("2 -", "LOGIN");
        opcaoMenu("0 -", "SAIR");
        borda();
        printf(BOLD "Escolha uma opcao: " RESET);
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                limparTela();
                criarConta(contas, &qtdContas);
                break;

            case 2:
                limparTela();
                topo("LOGIN");
                printf(CYAN "| " RESET "Conta: ");
                scanf("%d", &contaDigitada);
                limparBuffer();
                printf(CYAN "| " RESET "Senha: ");
                scanf("%d", &senhaDigitada);
                limparBuffer();
                borda();

                indiceLogin = -1;
                for (int i = 0; i < qtdContas; i++) {
                    if (contas[i].numero == contaDigitada && contas[i].senha == senhaDigitada) {
                        indiceLogin = i;
                        break;
                    }
                }

                if (indiceLogin == -1) {
                    erro("CONTA OU SENHA INVALIDA.");
                    pausar();
                    break;
                }

                sucesso("SESSAO INICIADA!");
                printf(BOLD "Bem-vindo, %s!\n" RESET, contas[indiceLogin].nome);
                pausar();

                do {
                    limparTela();
                    char titulo[40];
                    snprintf(titulo, sizeof(titulo), "OLA, %s", contas[indiceLogin].nome);
                    topo(titulo);
                    opcaoMenu("1 -", "SALDO");
                    opcaoMenu("2 -", "SAQUE");
                    opcaoMenu("3 -", "DEPOSITO");
                    opcaoMenu("4 -", "TRANSFERENCIA");
                    opcaoMenu("5 -", "EXTRATO");
                    opcaoMenu("0 -", "VOLTAR");
                    borda();
                    printf(BOLD "Escolha uma opcao: " RESET);
                    scanf("%d", &opcaoConta);
                    limparBuffer();

                    switch (opcaoConta) {
                        case 1:
                            printf(GREEN "\nSALDO ATUAL: R$ %.2f\n" RESET, contas[indiceLogin].saldo);
                            pausar();
                            break;

                        case 2:
                            printf(BOLD "\nValor do saque: R$ " RESET);
                            scanf("%f", &valor);
                            limparBuffer();

                            if (valor > contas[indiceLogin].saldo) {
                                erro("SALDO INSUFICIENTE.");
                            } else {
                                contas[indiceLogin].saldo -= valor;
                                snprintf(movimento, sizeof(movimento), "Saque: R$ %.2f", valor);
                                adicionarMovimento(&contas[indiceLogin], movimento);
                                sucesso("SAQUE REALIZADO COM SUCESSO!");
                            }
                            pausar();
                            break;

                        case 3:
                            printf(BOLD "\nValor do deposito: R$ " RESET);
                            scanf("%f", &valor);
                            limparBuffer();

                            if (valor <= 0) {
                                erro("Valor invalido.");
                            } else {
                                contas[indiceLogin].saldo += valor;
                                snprintf(movimento, sizeof(movimento), "Deposito: R$ %.2f", valor);
                                adicionarMovimento(&contas[indiceLogin], movimento);
                                sucesso("DEPOSITO REALIZADO COM SUCESSO!");
                            }
                            pausar();
                            break;

                        case 4: {
                            printf(BOLD "\nConta de destino: " RESET);
                            scanf("%d", &contaDigitada);
                            limparBuffer();

                            int destino = -1;
                            for (int i = 0; i < qtdContas; i++) {
                                if (contas[i].numero == contaDigitada) {
                                    destino = i;
                                    break;
                                }
                            }

                            if (destino == -1 || destino == indiceLogin) {
                                erro("CONTA DE DESTINO INVALIDA.");
                                pausar();
                                break;
                            }

                            printf(BOLD "Valor da transferencia: R$ " RESET);
                            scanf("%f", &valor);
                            limparBuffer();

                            if (valor > contas[indiceLogin].saldo) {
                                erro("SALDO INSUFICIENTE PARA TRANSFERENCIA.");
                            } else {
                                contas[indiceLogin].saldo -= valor;
                                contas[destino].saldo += valor;
                                snprintf(movimento, sizeof(movimento), "Transferencia para conta %d: R$ %.2f", contas[destino].numero, valor);
                                adicionarMovimento(&contas[indiceLogin], movimento);
                                snprintf(movimento, sizeof(movimento), "Recebido de conta %d: R$ %.2f", contas[indiceLogin].numero, valor);
                                adicionarMovimento(&contas[destino], movimento);
                                sucesso("TRANSFERENCIA REALIZADA COM SUCESSO!");
                            }
                            pausar();
                            break;
                        }

                        case 5:
                            limparTela();
                            mostrarExtrato(&contas[indiceLogin]);
                            pausar();
                            break;

                        case 0:
                            aviso("VOLTANDO AO MENU PRINCIPAL...");
                            break;

                        default:
                            erro("OPCAO INVALIDA.");
                            pausar();
                            break;
                    }

                } while (opcaoConta != 0);

                indiceLogin = -1;
                break;

            case 0:
                limparTela();
                printf(CYAN BOLD "\n   Obrigado por usar o BANCO DO POVO!\n\n" RESET);
                break;

            default:
                erro("OPCAO INVALIDA.");
                pausar();
                break;
        }
    } while (opcao != 0);

    return 0;
}