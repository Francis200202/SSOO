/*Implemente un programa que cree un vector de 10 elementos relleno con números aleatorios
entre 1 y 9. Reparta ese vector entre 2 hebras o 5 hebras a partes iguales, según se indique por linea
de argumentos un 2 o un 5, de forma que cada hebra sume la parte del vector que le corresponda y
se lo devuelva al hilo principal, el cual mostrará la suma de los resultados devuelto por las hebras
creadas.
Ejemplo de invocación del programa para crear 4 hebras que se repartan el vector: ./a.out 5*/
#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>

void *function2(void*);
void *function5(void*);

int main(int argn, char **argp){
	int n, suma;
	int v[10];
	if(argn<2){										//Comprueba numero de argumentos introducidos al ejecutar
		printf("Numero de argumentos invalido\n");
		exit(EXIT_FAILURE);
	}
	n= atoi(argp[1]);

	pthread_t hebra[n];								//Vector de hilos
	int *ret;

	srand(time(NULL));								
	for(int i=0;i<10;i++){							//Se asigna un numero aleat a cada posicion del vector v
		v[i]=rand()%10;
		printf("V[%d]: %d\n", i, v[i]);				//Se imprime el vector
	}

	switch(n){										//Segun n se crean 2 o 5 hebras para sumar los elementos del vector
		case 2:
			for(int i=0;i<2;i++){
				if( pthread_create(&(hebra[i]), NULL, function2, (void *)&v[i*5])) //Se crean 2 hilos, el primero recibe un puntero a la ps 0 y el segundo a la pos 5(i*5=1*5=5)
    			{
	  				fprintf(stderr, "Error creating thread\n");
	  				exit(EXIT_FAILURE); 
				}
			}
		break;
		case 5:
			for(int i=0;i<5;i++){
				if( pthread_create(&(hebra[i]), NULL, function5, (void *)&v[i*2])) //Se crean 5 hilos, 1: puntero pos 0, 2: pos2, 3: pos4, 4 pos 6, 5: pos 8
    			{
	  				fprintf(stderr, "Error creating thread\n");
	  				exit(EXIT_FAILURE); 
				}
			}
		break;
		default:									//Si n no es ni 2 ni 5, no se ha introducido un argumento valido
			printf("Argumento invalido\n");
			exit(EXIT_FAILURE);
	}
	for(int i=0; i<n; i++){
		if(pthread_join(hebra[i], (void**) &ret))	//Esperamos la finalizacion de los hijos y recibimos la suma de 2 o 5 enteros
		{
		  perror("Fallo en pthread_join()...\n");		
		  exit(EXIT_FAILURE);
		}
		printf("Hebra%d:%d  ", i+1,*ret);
		suma+= *ret;
	}
	printf("\nSUMA TOTAL: %d\n", suma);				//Se imprime la suma total de todas las sumas devueltas por los hilos
	exit(EXIT_SUCCESS);
}

void *function2(void* v){				//Funcion que hace la suma desde el puntero que recibe hasta las 5 pos siguientes
	int *aux;
	aux= (int *)v;						//Casting a entero del parametro de entrada
	int *suma= malloc(sizeof(int));		//Reserva de memoria de lo que vamos a devolver para que no se produzca un error
	for(int i=0;i<5;i++){
		*suma+= aux[i];
	}
	pthread_exit((void *)suma); 		//Se devuelve la suma
}

void *function5(void* v){				//Funcion que hace la suma desde el puntero que recibe hasta las 2 pos siguientes
	int *aux= (int *)v;
	int *suma= malloc(sizeof(int));		//Casting a entero del parametro de entrada
	for(int i=0;i<2;i++){
		*suma+= aux[i];
	}
	pthread_exit((void *)suma);			//Se devuelve la suma
}