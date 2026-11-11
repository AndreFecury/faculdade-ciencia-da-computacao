#include <stdio.h>
#include <string.h>

#define TAM 50

/* Le uma linha do teclado sem estourar o buffer. Retorna 0 em EOF. */
int lerTexto(char *dest, int tam) {
    int c;
    if (fgets(dest, tam, stdin) == NULL) {
        dest[0] = '\0';
        return 0;
    }
    if (strchr(dest, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    dest[strcspn(dest, "\n")] = '\0';
    return 1;
}

/* Le uma nota valida (0 a 10). */
float lerNota(const char *msg) {
    char linha[TAM];
    float v;
    while (1) {
        printf("%s", msg);
        if (!lerTexto(linha, sizeof(linha))) return 0;
        if (sscanf(linha, "%f", &v) == 1 && v >= 0 && v <= 10) return v;
        printf("Nota invalida! Digite um valor entre 0 e 10.\n");
    }
}

/* Le um registro "nome;nota1;nota2" do arquivo. Retorna 1 se leu. */
int lerRegistro(FILE *arq, char *nome, float *n1, float *n2) {
    char linha[200];
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        if (sscanf(linha, "%49[^;];%f;%f", nome, n1, n2) == 3) return 1;
    }
    return 0;
}

int main() {
    int opcao, achou, qtd;
    char entrada[TAM], nome[TAM], buscar[TAM], lido[TAM];
    float nota1, nota2, media, n1, n2;
    FILE *arq, *temp;

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Cadastrar aluno\n");
        printf("2 - Listar alunos\n");
        printf("3 - Alterar aluno\n");
        printf("4 - Excluir aluno\n");
        printf("5 - Sair\n");
        printf("Escolha uma opcao: ");

        if (!lerTexto(entrada, sizeof(entrada))) break; /* EOF */
        if (sscanf(entrada, "%d", &opcao) != 1) opcao = 0;

        switch (opcao) {

        /* ----- 1: CADASTRAR ----- */
        case 1:
            printf("Nome do aluno: ");
            lerTexto(nome, sizeof(nome));
            if (nome[0] == '\0') {
                printf("O nome nao pode ser vazio!\n");
                break;
            }
            if (strchr(nome, ';') != NULL) {
                printf("O nome nao pode conter ';'!\n");
                break;
            }
            nota1 = lerNota("Nota 1: ");
            nota2 = lerNota("Nota 2: ");

            arq = fopen("alunos.txt", "a");
            if (arq == NULL) {
                printf("Erro ao abrir o arquivo!\n");
            } else {
                fprintf(arq, "%s;%.2f;%.2f\n", nome, nota1, nota2);
                fclose(arq);
                printf("Aluno cadastrado!\n");
            }
            break;

        /* ----- 2: LISTAR ----- */
        case 2:
            arq = fopen("alunos.txt", "r");
            if (arq == NULL) {
                printf("Nenhum aluno cadastrado!\n");
                break;
            }
            qtd = 0;
            printf("\n===== ALUNOS =====\n");
            while (lerRegistro(arq, lido, &n1, &n2)) {
                media = (n1 + n2) / 2;
                printf("Nome: %s\n", lido);
                printf("Nota 1: %.2f\n", n1);
                printf("Nota 2: %.2f\n", n2);
                printf("Media: %.2f\n", media);
                printf("Situacao: %s\n", media >= 7 ? "Aprovado" : "Reprovado");
                printf("-------------------\n");
                qtd++;
            }
            fclose(arq);
            if (qtd == 0) printf("Nenhum aluno cadastrado!\n");
            break;

        /* ----- 3: ALTERAR ----- */
        case 3:
        /* ----- 4: EXCLUIR ----- */
        case 4:
            printf(opcao == 3 ? "Nome do aluno que deseja alterar: "
                              : "Nome do aluno que deseja excluir: ");
            lerTexto(buscar, sizeof(buscar));

            arq = fopen("alunos.txt", "r");
            if (arq == NULL) {
                printf("Nenhum aluno cadastrado!\n");
                break;
            }
            temp = fopen("temp.txt", "w");
            if (temp == NULL) {
                fclose(arq);
                printf("Erro ao criar arquivo temporario!\n");
                break;
            }

            achou = 0;
            while (lerRegistro(arq, lido, &n1, &n2)) {
                if (strcmp(lido, buscar) == 0) {
                    achou = 1;
                    if (opcao == 4) continue; /* nao copia: exclui */
                    n1 = lerNota("Nova nota 1: ");
                    n2 = lerNota("Nova nota 2: ");
                }
                fprintf(temp, "%s;%.2f;%.2f\n", lido, n1, n2);
            }
            fclose(arq);
            fclose(temp);

            if (achou) {
                remove("alunos1.txt");
                rename("temp.txt", "alunos1.txt");
                printf(opcao == 3 ? "Aluno alterado!\n" : "Aluno excluido!\n");
            } else {
                remove("temp.txt");
                printf("Aluno nao encontrado!\n");
            }
            break;

        case 5:
            printf("\nPrograma encerrado!\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }

    } while (opcao != 5);

    return 0;
}