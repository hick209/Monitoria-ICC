#include <time.h>
#include <stdio.h>
#include <windows.h>

#define TRUE  1
#define FALSE 0

#define TAMANHO_MAXIMO 50


int main() {

	int i;
	int pontuacao = 0;
	int acertou   = TRUE;

	int elemento[TAMANHO_MAXIMO];
	int elementoDoJogador;


	// Sorteia os números da sequência
	for (i = 0; i < TAMANHO_MAXIMO; i++) {
		elemento[i] = rand() % 10;
	}

	while (acertou == TRUE) {
		pontuacao++;

		// Apaga a tela
		system("cls");
		printf("--- Jogo da Memoria ---\n");
		printf("  Numeros:\n");
		Sleep(1000);

		// Mostrar os números
		for (i = 0; i < pontuacao; i++) {
			// Mostra número i
			printf("\t%d", elemento[i]);
			Sleep(500);
		}
		// Espera 5 segundos
		Sleep(5000); // em milisegundos

		// Apaga a tela
		system("cls");

		// Pede a sequência para o jogador
		printf("\n Entre os numeros!\n");
		for (i = 0; i < pontuacao; i++) {
			// Lê o que o jogador entrou
			printf("\t");
			scanf("%d", &elementoDoJogador);

            // Analisa se o elemento entrado pelo jogador está correto
			if (elementoDoJogador != elemento[i]) {
				// Errou!
				printf("Entrou um numero errado!\n");
				printf("Deveria ser %d, voce digitou %d\n", elemento[i], elementoDoJogador);

				// Marca que o jogador errou
				acertou = FALSE;

				// Sai de dentro do loop for
				break;
			}
		}
	}

    // Imprimi a pontuação do jogador
    printf("\n\n\tVocê fez %d ponto(s)", pontuacao);

	return 0;
}

