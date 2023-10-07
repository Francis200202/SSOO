/*Problema productor-consumidor
El sistema está obligado a impedir la superposición de las operaciones sobre los datos, es decir,
sólo un agente (productor o consumidor) puede acceder al buffer en un momento dado (así el
productor no sobrescribe un elemento que esté leyendo el consumidor, y viceversa). Estaríamos
hablando de la sección crítica
Si suponemos que el buffer es limitado y está completo, el productor debe esperar hasta que el
consumidor lea al menos un elemento para así poder seguir almacenando datos. En el caso de que el
buffer esté vacío el consumidor debe esperar a que se escriba información nueva por parte del
productor
b) Implemente el problema para hilos, teniendo en cuenta que la sección crítica va a ser un
array de enteros con una capacidad de 3 elementos y donde habrá 3 productores y 3
consumidores, que podrán producir-consumir 1 elemento cada uno*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#define NELEM 3

sem_t s;
sem_t n;
sem_t e;

int v[NELEM];

void *producir(void* arg){

	sem_wait(&e);
	sem_wait(&s);
	v[*(int *)arg]=1;
	printf("++PRODUCIDO--> elemento %d    ", *(int *)arg+1);
	for(int i=0; i<NELEM; i++){
		printf("v[%d]= %d   ", i+1, v[i]);
	}
	printf("\n");
	sem_post(&s);
	sem_post(&n);

	pthread_exit(NULL);

}

void *consumir(void *arg){

	sem_wait(&n);
	sem_wait(&s);
	v[*(int *)arg]=0;
	printf("--CONSUMIDO--> elemento %d    ", *(int *)arg+1);
	for(int i=0; i<NELEM; i++){
		printf("v[%d]= %d   ", i+1, v[i]);
	}
	printf("\n");
	sem_post(&s);
	sem_post(&e);

	pthread_exit(NULL);
	
}


int main(){
	
	pthread_t *productor, *consumidor;
	int x[NELEM];

	productor= (pthread_t *) malloc ((NELEM)*sizeof(pthread_t));
	consumidor= (pthread_t *) malloc ((NELEM)*sizeof(pthread_t));

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

	for(int i=0; i<NELEM; i++){
		x[i]= i;
		if(pthread_create(&(productor[i]), NULL, (void *)producir, (void *) &x[i])){
			printf("Error creating thread productor\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i=0; i<NELEM; i++){
		x[i]= i;
		if(pthread_create(&(consumidor[i]), NULL, (void *)consumir, (void *) &x[i])){
			printf("Error creating thread consumidor\n");
			exit(EXIT_FAILURE);
		}
	}
	
	for(int i=0; i<NELEM; i++){
		if(pthread_join(productor[i], NULL)){
			printf("Error joining thread productor\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i=0; i<NELEM; i++){
		if(pthread_join(consumidor[i], NULL)){
			printf("Error joining thread consumidor\n");
			exit(EXIT_FAILURE);
		}
	}

	sem_destroy(&s);
	sem_destroy(&n);
	sem_destroy(&e);
	
}