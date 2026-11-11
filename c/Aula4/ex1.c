#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

struct Aluno
{
    char nome[20];
    int idade;
    int codigo;
};

int main(void)
{
    struct Aluno a;

    setlocale(LC_ALL, "Portuguese");
    printf("Digite o nome do aluno: ");
    scanf("%s", a.nome);
    printf("Digite a idade do aluno: ");
    scanf("%d", &a.idade);
    printf("Digite o código do aluno: ");
    scanf("%d", &a.codigo);
    printf("\nNome: %s\nIdade: %d\nCódigo: %d\n", a.nome, a.idade, a.codigo);

    return 0;
}

