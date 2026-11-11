#include <stdio.h>
#include <locale.h>

int main(void) {
    int idade, tempo_de_servidor, aposentadoria_idade, aposentadoria_tempo;

    printf("Informe sua idade: ");
    scanf("%d", &idade);

    printf("Informe o tempo de servico: ");
    scanf("%d", &tempo_de_servidor);

    if (idade >= 65 || tempo_de_servidor >= 30) {
        printf("Voce ja pode se aposentar");
    } else {
        aposentadoria_idade = 65 - idade;
        aposentadoria_tempo = 30 - tempo_de_servidor;
        printf("Voce ainda precisa trabalhar por mais %d anos para se aposentar por idade ou %d anos por tempo de servico", aposentadoria_idade, aposentadoria_tempo);
    }
    return 0;
}