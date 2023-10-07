/*Problema productor-consumidor
El sistema está obligado a impedir la superposición de las operaciones sobre los datos, es decir,
sólo un agente (productor o consumidor) puede acceder al buffer en un momento dado (así el
productor no sobrescribe un elemento que esté leyendo el consumidor, y viceversa). Estaríamos
hablando de la sección crítica
Si suponemos que el buffer es limitado y está completo, el productor debe esperar hasta que el
consumidor lea al menos un elemento para así poder seguir almacenando datos. En el caso de que el
buffer esté vacío el consumidor debe esperar a que se escriba información nueva por parte del
productor
a) Implemente el problema para hilos, teniendo en cuenta que la sección crítica va a ser un
array de enteros con una capacidad de 5 elementos y donde habrá un productor y un
consumidor. Se podrán producir-consumir 10 elementos*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#define NELEM 5
#define PRODCON 10

sem_t s;
sem_t n;
sem_t e;

int v[NELEM];

void *producir(void* arg){
	for(int x=0; x<(PRODCON/NELEM); x++){
		for(int i=0; i<NELEM; i++){
			sem_wait(&e);
			sem_wait(&s);
			v[i]=1;
			printf("++PRODUCIDO--> elemento %d    ", i+1);
			for(int i=0; i<NELEM; i++){
				printf("v[%d]= %d   ", i+1, v[i]);
			}
			printf("\n");
			sem_post(&s);
			sem_post(&n);
		}
	}

	pthread_exit(NULL);
}

void *consumir(void *arg){
	for(int x=0; x<(PRODCON/NELEM); x++){
		for(int i=0; i<NELEM; i++){
			sem_wait(&n);
			sem_wait(&s);
			v[i]=0;
			printf("--CONSUMIDO--> elemento %d    ", i+1);
			for(int i=0; i<NELEM; i++){
				printf("v[%d]= %d   ", i+1, v[i]);
			}
			printf("\n");
			sem_post(&s);
			sem_post(&e);
		}
	}

	pthread_exit(NULL);
}


int main(){
	
	pthread_t productor, consumidor;

	for(int i=0; i<NELEM; i++){
		v[i]=0;
		printf("v[%d]= %d   ", i+1, v[i]);
	}
	printf("\n");

	if(sem_init(&s, 0, 1)){
		printf("sem_ init error...\n");
		printf("errno value= %d definido como %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(sem_init(&n, 0, 0)){
		printf("sem_init error...\n");
		printf("errno value= %d definido como %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(sem_init(&e, 0, NELEM)){
		printf("sem_init error...\n");
		printf("errno value= %d definido como %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&productor, NULL, (void *)producir, NULL)){
		printf("Error creating thread productor\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&consumidor, NULL, (void *)consumir, NULL)){
		printf("Error creating thread consumidor\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_join(productor, NULL)){
		printf("Error joining thread productor\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(consumidor, NULL)){
		printf("Error joining thread consumidor\n");
		exit(EXIT_FAILURE);
	}

	sem_destroy(&s);
	sem_destroy(&n);
	sem_destroy(&e);
}