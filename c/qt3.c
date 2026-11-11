#include <stdio.h>

int main() {
    float numero;

    printf("Digite um numero: ");
    scanf("%f", &numero);

    if (numero > 100) {
        printf("O numero e maior que 100!\n");
    } else {
        printf("O numero nao e maior que 100.\n");
    }

    return 0;
}
