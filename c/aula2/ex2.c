#include <stdio.h>
#include <locale.h>

int main(void) {
    char nome[10];
    int nascimento, idade, ano_atual = 2026;

    printf("\nInforme seu nome: ");
    scanf("%9s", nome);
    printf("\nInforme sua data de nascimento: ");
    scanf("%i", &nascimento);
    
    idade = ano_atual - nascimento;

    printf("%s, %i anos", nome, idade);

    return 0;
}