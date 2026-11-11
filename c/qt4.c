#include <stdio.h>

int main() {
    int ano_nascimento, ano_atual, idade;

    printf("Digite o ano de nascimento: ");
    scanf("%d", &ano_nascimento);

    printf("Digite o ano atual: ");
    scanf("%d", &ano_atual);

    idade = ano_atual - ano_nascimento;
    printf("Idade: %d anos\n", idade);

    return 0;
}