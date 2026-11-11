#include <stdio.h>
#include <locale.h>
int main() {
    setlocale(LC_ALL, "Portuguese");

     int n;

    printf("informe um valor: ");
    scanf("%d", &n);

    if (n > 0) {
        printf("O numero e positivo\n");
    } else if (n < 0) {
        printf("O numero e negativo\n");
    } else {
        printf("O numero e neutro\n");
    }

    return 0;
}