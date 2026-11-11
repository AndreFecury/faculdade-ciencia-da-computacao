#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Portuguese");

    float nota;
    int falta;

    printf("Informe a nota do aluno: ");
    scanf("%f", &nota);
    printf("Informe a quantidade de faltas do aluno: ");
    scanf("%d", &falta);

    if (nota < 0 || nota > 10 || falta < 0) {
        printf("Nota ou quantidade de faltas inválida.\n");
    } else if (nota >= 8.6) {
        printf("Conceito: %c\n", falta > 20 ? 'B' : 'A');
    } else if (nota >= 7.1) {
        printf("Conceito: %c\n", falta > 20 ? 'C' : 'B');
    } else if (nota >= 5.6) {
        printf("Conceito: %c\n", falta > 20 ? 'D' : 'C');
    } else if (nota >= 4.0) {
        printf("Conceito: %c\n", falta > 20 ? 'E' : 'E');
    } else {
        printf("Conceito: E\n");
    }

    return 0;
}