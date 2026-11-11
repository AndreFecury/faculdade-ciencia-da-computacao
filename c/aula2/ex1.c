#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(){

    int a, b, c ;

    setlocale(LC_ALL, "portuguese");

    printf("\nInforme um valor para A: ");
    scanf("%i", &a);

    printf("\nInforme um valor para B: ");
    scanf("%i", &b);
    c = a;
    a = b;
    b = c; 
    printf("\nO valor de A e: %i e B e: %i", a, b);
}