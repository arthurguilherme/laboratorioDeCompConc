/*
 * lab1-atividade5.c
 *
 *  Created on: 14 de dez de 2020
 *      Author: Arthur Guilherme Souza
 *      DRE: 112018654
 *  Disciplina: Computacao Concorrente
 *  Prof.: Silvana Rossetto
 *  Modulo 1 - Laboratorio: 2
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat; // matriz de entrada
float *vet; // vetor de entrada
float *saida; // vetor de saida
int nthreads; // numero de threads
double inicio, fim, tempo;

typedef struct {
	int id; // identificador do elemento que a thread ira processar
	int dim; // dimensao das estruturas de entrada
} tArgs;

// funcao que as threads executarao
void* tarefa(void *arg) {
	tArgs *args = (tArgs*) arg;
	printf("Thread %d\n", args->id);
	for (int i = (args->id); i < (args->dim); i += nthreads) {
		for (int j = 0; j < (args->dim); j++) {
			saida[i] += mat[i*(args->id) + j] * vet[j];
		}
	}
	pthread_exit(NULL);
}

// fluxo principal
int main(int argc, char **argv) {

	int dim; // dimensao da matriz de entrada
	pthread_t *tid; // identificadores das threads no sistema
	tArgs *args; // identificadores locais das threads e dimensao

	// leitura e avaliação dos parametros de entrada
	if (argc < 3) {
		printf("Digite: %s <dimensao da matriz><numero de threads>\n", argv[0]);
		return 1;
	}

	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);

	if (nthreads > dim) {
		nthreads = dim;
	}

	// alocacao de memoria para estruturas de dados
	mat = (float*) malloc(sizeof(float) * dim * dim);
	if (mat == NULL) {
		printf("ERRO--malloc\n");
		return 2;
	}
	vet = (float*) malloc(sizeof(float) * dim);
	if (vet == NULL) {
		printf("ERRO--malloc\n");
		return 2;
	}
	saida = (float*) malloc(sizeof(float) * dim);
	if (saida == NULL) {
		printf("ERRO--malloc\n");
		return 2;
	}

	// inicializacao das estruturas de dados
	GET_TIME(inicio);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			mat[i * dim + j] = 1;
		}
		vet[i] = 1;
		saida[i] = 0;
	}
	GET_TIME(fim);
	double delta;
	delta = fim - inicio;
	printf("Tempo de inicializacao das estruturas: %lf", delta);
	// multiplicacao da matriz pelo vetor

	//		alocacao das estruturas
	tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
	if (tid == NULL) {
		puts("Erro--malloc!");
		return 2;
	}
	args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
	if (args == NULL) {
		puts("Erro--malloc!");
		return 2;
	}

	// 		criacao das threads
	GET_TIME(inicio);
	for (int i = 0; i < nthreads; i++) {
		(args+i)->id = i;
		(args+i)->dim = nthreads;
		if (pthread_create(tid+i, NULL, tarefa, (void*) (args + i))) {
			puts("Erro--pthread_create");
			return 3;
		}
	}

	// 		espera pelo termina das threads
	for (int i = 0; i < nthreads; i++) {
		pthread_join(*(tid+i), NULL);
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de criacao das threads, execucao da multiplicacao, e termino das threads: %lf", delta);


	GET_TIME(inicio);
	//	exibicao dos resultados
	puts("Vetor de saida:");
	for (int j = 0; j < dim; j++) {
		printf("%f ", saida[j]);
	}
	puts("");

	// liberacao da memoria
	free(mat);
	free(vet);
	free(saida);
	free(args);
	free(tid);
	GET_TIME(fim);
	printf("Tempo de finalizacao: %lf", delta);

	return 0;
}
