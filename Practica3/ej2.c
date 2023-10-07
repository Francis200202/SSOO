/*Práctica 3 de SSOO Juan Carlos Fernández Caballero, jfcaballero@uco.es
Ejemplos de salida del ejercicio:
4.1.2 Ejercicio 2
Implemente un programa que cree un número N de hilos, donde N será un argumento al
programa por la linea de comandos.
Tenga en cuenta los siguientes supuestos y condiciones:
 -Los N hilos comparten dos variables, una llamada "par" e "impar" inicializadas a 0.
 -Los hilos deben recibir un valor entero de 1 a N que indica el orden en el cuál se ha creado
el hilo. Así, el primer hilo creado recibe 1, el segundo 2, etc.
 -Cada hilo debe generar 5 números aleatorios entre 0 y 10.
 -Los hilos de índice de creación par deben ir sumando a la variable "par" los números que
generan.
 -Los hilos de índice de creación impar deben ir sumando a la variable "impar" los números
que generan.
 -Una vez finalizado cada hilo, éste debe devolver al programa principal la suma total de
números que ha generado*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int par=0, impar=0;
pthread_mutex_t mtx= PTHREAD_MUTEX_INITIALIZER;

void *funcion(void *N);

int main(int argc, char *argv[]){
	int N;

	if(argc!=2){
		printf("Error en el numero de argumentos, debe tener esta estructura--> ./a.out (numero entero)\n");
		exit(EXIT_FAILURE);
	}

	N= atoi(argv[1]);

	if(N<=0){
		printf("ERROR, el argumento debe ser un numero entero\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	pthread_t *hilo;
	int *v;
	int *suma;
	int sumapar=0, sumaimpar=0;
	hilo= (pthread_t *) malloc ((N)*sizeof(pthread_t));
	v= (int *) malloc ((N)*sizeof(int));
	int x=0;

	for(int i=0;i<N;i++){
		v[i]=i+1;
		if(pthread_create(&(hilo[i]), NULL, (void *)funcion, (void *)&v[i])){
			printf("Errro creating thread\n");
			exit(EXIT_FAILURE);
		}
	}

	for(int i=0; i<N; i++){
		if(pthread_join(hilo[i], (void **)&suma)){
			printf("Error joining thread\n");
			exit(EXIT_FAILURE);
		}
		printf("MAIN()...La hebra de orden de creacion %d devolvio el valor de suma total: %d\n", i+1, *suma);
		if((i+1)%2==0){
			sumapar+=*suma;
		}else{
			sumaimpar+=*suma;
		}
	}
	
	printf("\nMAIN()...Valor de la variable compartida impar: %d\n", impar);
	printf("MAIN()...Suma de los valores devueltos por las hebras de creacion impares: %d\n", sumaimpar);

	printf("\nMAIN()...Valor de la variable compartida par: %d\n", par);
	printf("MAIN()...Suma de los valores devueltos por las hebras de creacion pares: %d\n", sumapar);
}

void *funcion(void *x){
	int n= *(int *)x;
	int num, s;
	int *suma= malloc (sizeof(int));
	
	if(n%2==0){
		for(int i=0;i<5;i++){
			num= rand()%11;
			s= pthread_mutex_lock(&mtx);
			if(s!=0){
				printf("mutex lock error...\n");
				pthread_exit(NULL);
			}
			par+= num;
			s= pthread_mutex_unlock(&mtx);
			if(s!=0){
				printf("mutex unlock error");
				pthread_exit(NULL);
			}
			*suma+= num;
		}
	}else{
		for(int i=0;i<5;i++){
			num= rand()%11;
			s= pthread_mutex_lock(&mtx);
			if(s!=0){
				printf("mutex lock error...\n");
				pthread_exit(NULL);
			}
			impar+= num;
			s= pthread_mutex_unlock(&mtx);
			if(s!=0){
				printf("mutex unlock error");
				pthread_exit(NULL);
			}
			*suma+= num;
		}
	}
	pthread_exit((void *)suma);
}