/*
 * aula3.c: Soma todos elementos de um vetor de inteiro
 *
 *  Created on: 14 de dez de 2020
 *      Author: Arthur Guilherme Souza
 *      DRE: 112018654
 *  Disciplina: Computacao Concorrente
 *  Prof.: Silvana Rossetto
 *  Modulo 1 - Aula: 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

long long int N; // numero de elementos do vetor
int *vetor; // vetor de entrada com dimensao N

// fluxo principal
int main(int argc, char **argv) {

	double somaSeq = 0; // soma sequencial
	double ini, fim; // tomada de tempo

	// recebe os parametros de entrada (dimensao do vetor, numero de threads
	if(argc<2){
		fprintf(stderr, "Digite: %s <numero de elementos da serie>\n", argv[0]);
		return 1;
	}
	N = atoll(argv[1]);

	// aloca o vetor de entrada
	vetor = (int *) malloc(sizeof(int)*N);
	if(vetor == NULL){
		fprintf(stderr, "Erro--malloc\n");
		return 2;
	}

	// preenche o vetor de entrada
	for(long long int i = 0; i < N; i++){
		vetor[i] = 1+2*i;
	}

	// soma sequencial dos elementos
	GET_TIME(ini);
	for(long long int i = 0; i<N; i+=2){
		somaSeq += 4./vetor[i];
		somaSeq -= 4./vetor[i+1];
	}
	GET_TIME(fim);
	printf("Tempo sequencial: %lf\n", fim - ini);

	//	exibir os resultados
	printf("Valor de PI aproximado pela serie: %.15f\n", somaSeq);
	printf("Valor da constante M_PI: %.15f\n", M_PI);
	printf("Diferença percentual: %.15f%%", fabs(100-100*somaSeq/M_PI));

	//	 libera as areas de memoria alocadas
	free(vetor);

	return 0;
}
