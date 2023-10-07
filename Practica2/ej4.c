/*Implemente un programa que cree dos hebras y cada una incremente 10000 veces en un bucle
una variable global (recuerde que la variable global, al estar en el mismo espacio de memoria para
las dos hebras, es compartida, y que su uso es “peligroso”). Imprima al final del programa principal
el valor de la variable (en cada ejecución posiblemente obtenga un valor diferente a 20000 –
problema de concurrencia –). Intente razonar el resultado, el cual le servirá como concepto
introductorio de la siguiente práctica de la asignatura.*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2

int global=0;

void *function();

int main(){

	pthread_t hebra[N];

	for(int i=0;i<N;i++){
		if(pthread_create(&(hebra[i]), NULL, (void *)function, NULL)){
			fprintf(stderr, "Error creating thread\n");
			exit(EXIT_FAILURE);
		}
	}

	for(int i=0;i<N;i++){
		if(pthread_join(hebra[i], NULL)){
			fprintf(stderr, "Error joining thread\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("VARIABLE GLOBAL: %d\n", global);
}

void *function(){
	for(int i=0;i<10000;i++){
		global++;
	}
}