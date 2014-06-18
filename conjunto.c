#include <stdio.h>

#define TRUE  1
#define FALSE 0

#define SIZE  5


int main() {
    int vetor[SIZE];

    int i, j;
    int jaExiste = FALSE;

    // Preenche o vetor
    for (i = 0; i < SIZE; i++) {
        printf("Entre o %d-esimo valor: ", i+1);
        scanf("%d", &vetor[i]);
    }

    // Imprimi elementos não repetidos
    printf("\nElementos sem repetição:\n\t");
    for (i = 0; i < SIZE; i++) {
        jaExiste = FALSE;
        for (j = 0; j < i; j++) {
            if (vetor[i] == vetor[j]) {
                jaExiste = TRUE;
            }
        }
        
        if (jaExiste == FALSE) {
            printf("%d ", vetor[i]);
        }
    }

    return 0;
}

