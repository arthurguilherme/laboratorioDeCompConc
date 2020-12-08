/*
 * lab1-atividade5.c
 *
 *  Created on: 6 de dez de 2020
 *      Author: Arthur Guilherme Souza
 *  Disciplina: Computacao Concorrente
 *  Prof.: Silvana Rossetto
 *  Módulo 1 - Laboratório: 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

// declaração do vetor de N elementos (10 < N < 100)
int vetor[89];

void *IncrementaVetor (void* arg){

	int idThread = * (int*) arg; 
	
	for(int N = idThread; N<89; N+=2){
		vetor[N]++;
		// printf("Thread%d\n",idThread); // para visualizar a ordem em que as threads sao executadas
	}
	
	pthread_exit(NULL);
}

int main(int argc, char **argv) {

	// população do vetor de N elementos (10 < N < 100)
	int N; // variável auxiliar 
	for(N = 11; N<100; N++)
		vetor[N-11] = N;

	// impressão do estado inicial do vetor
	printf("Impressão do estado inicial do vetor:\n");
	for(N = 0; N<89; N++)
		printf("vetor[%d] = %d, ", N, vetor[N]);
	printf("\n");
	

	pthread_t tid_sistema[NTHREADS]; // identificadores das threads no sistema
	int thread; // variavel auxiliar
	int tid_local[NTHREADS]; // identificadores locais das threads
	

	for(thread=0; thread<NTHREADS; thread++) {
	    tid_local[thread] = thread;
	    if(pthread_create(&tid_sistema[thread], NULL, IncrementaVetor, (void*) &tid_local[thread])) {
	      printf("--ERRO: pthread_create()\n"); exit(-1);
	    }
	}

	// espera todas as threads terminarem
	for (thread=0; thread<NTHREADS; thread++) {
		if (pthread_join(tid_sistema[thread], NULL)) {
         		printf("--ERRO: pthread_join() \n"); exit(-1); 
		}
	}
	
	// impressão do estado final do vetor
	printf("\nImpressão do estado final do vetor:\n");
	for(int N = 0; N<89; N++)
		printf("vetor[%d] = %d, ", N, vetor[N]);
	printf("\n");
	
	pthread_exit(NULL);
	
}
