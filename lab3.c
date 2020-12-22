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
int nthreads;
double *vetor; // vetor de entrada com dimensao N

void *tarefa(void *arg){
	long int id = (long int) arg; // identificador da thread
	double *somaLocal; // variavel local da soma de elementos da serie alternada
	double tamBloco = N/nthreads; // tamanho do bloco de cada thread
	double ini = (long long int) id * tamBloco;  // elemento inicial do bloco da thread
	double fim; // elemento final (nao processado) do bloco da thread

	somaLocal = (double*) malloc(sizeof(double));
	if(somaLocal==NULL){
		fprintf(stderr, "Erro--malloc\n");
		exit(1);
	}
	*somaLocal = 0;

	if(id == nthreads - 1) fim = N;
	else fim = ini + tamBloco; // trata o resto se houver

	// soma os elementos do bloco da thread
	for(long long int i = ini; i<fim; i+=2){
			*somaLocal += 4./vetor[i];
			*somaLocal -= 4./vetor[i+1];
	}

	// retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}


// fluxo principal
int main(int argc, char **argv) {

	double somaSeq = 0; // soma sequencial
	double somaConc = 0; // soma concorrente
	double ini, fim; // tomada de tempo
	pthread_t *tid; // identificadores das threads no sistema
	double *retorno; // valor de retorno das threads

	// recebe e valida os parametros de entrada (dimensao do vetor, numero de threads
	if(argc<3){
		fprintf(stderr, "Digite: %s <numero de elementos da serie><numero de threads>\n", argv[0]);
		return 1;
	}
	N = atoll(argv[1]);
	nthreads = atoi(argv[2]);

	// aloca o vetor de entrada
	vetor = (double *) malloc(sizeof(double)*N);
	if(vetor == NULL){
		fprintf(stderr, "Erro--malloc\n");
		return 2;
	}

	// preenche o vetor de entrada
	for(long long int i = 0; i < N; i++){
		vetor[i] = 1+2*i;
	}

	// soma sequencial dos elementos da serie alternada
	GET_TIME(ini);
	for(long long int i = 0; i<N; i+=2){
		somaSeq += 4./vetor[i];
		somaSeq -= 4./vetor[i+1];
	}
	GET_TIME(fim);
	printf("Tempo sequencial: %.15lf\n", fim - ini);

// INICIA TEMPO CONCORRENTE

	// soma concorrente dos elementos
	GET_TIME(ini);
	tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
	if(tid == NULL){
		fprintf(stderr, "ERRO-malloc\n");
		return 2;
	}

	//	 criar as threads
	for(int i =0; i< nthreads; i++){
		if(pthread_create(tid +i, NULL, tarefa, (void*) i)){
			fprintf(stderr, "ERRO-pthread_create\n");
			return 3;
		}
	}

	//	 aguardar termino das threads e computar o valor final
	for(int i =0; i< nthreads; i++){
		if(pthread_join(*(tid+i), (void**) &retorno)){
			fprintf(stderr, "ERRO-pthread_create\n");
			return 3;
		}
		// soma global
		somaConc += *retorno;
	}
	GET_TIME(fim);
	printf("Tempo concorrente: %.15lf\n", fim - ini);

// TERMINA TEMPO CONCORRENTE

	//	exibir os resultados
	printf("Valor de PI aproximado pela serie sequencial: %.15f\n", somaSeq);
	printf("Valor de PI aproximado pela serie concorrente: %.15f\n", somaConc);
	printf("Valor da constante M_PI: %.15f\n", M_PI);
	printf("Diferença percentual entre constante e serie sequencial: %.15f%%\n", fabs(100-100*somaSeq/M_PI));
	printf("Diferença percentual entre constante e serie concorrente: %.15f%%\n", fabs(100-100*somaConc/M_PI));
	printf("Ganho de Desempenho / aceleracao: %.15f", somaSeq/somaConc);


	//	 libera as areas de memoria alocadas
	free(vetor);
	free(tid);

	return 0;
}
