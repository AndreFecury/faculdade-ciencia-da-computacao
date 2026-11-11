#include <stdio.h>

int main() {
    char primeiroNome[50];
    char ultimoNome[50];

    printf("Informe o primeiro nome: ");
    scanf("%s", primeiroNome);

    printf("Informe o ultimo nome: ");
    scanf("%s", ultimoNome);

    printf("%s, %s\n", primeiroNome , ultimoNome);

    return 0;
}