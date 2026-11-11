#include <stdio.h>
#include <locale.h>

int main(void) {
    int n;

    setlocale(LC_ALL, "Portuguese");

    printf("Par ou impar\n");
    printf("Digite um numero: ");
    scanf("%d", &n);

    if (n % 2 == 0) {
        printf("O numero %i e par", n);
    } else {
        printf("O numero %i e impar", n);
    }

    return 0;
}