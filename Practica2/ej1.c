
/*Implemente un programa que cree un número N de hebras. Cada hebra creará 2 números
aleatorios flotantes (consulte Moodle para la generación de aleatorios) y guardará su suma en una
variable para ello, que será devuelta a la hebra principal o llamadora (la que invocó
pthread_create()).
La hebra principal ira recogiendo los valores devueltos por las N hebras y los ira sumando en
una variable no global cuyo resultado mostrará al final por pantalla. Para ver que los resultados
finales son los que usted espera, muestre los 2 números que va creando cada hebra y su suma, de
forma que pueda comparar esas sumas parciales con la suma final que realizará el main() o hebra
principal. Utilice macros definidas y comprobación de errores en sus programas (errno y
comprobación de valores devueltos en cada llamada, con sus posibles alternativas).*/
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

#define N 4

void * function();

int main(){
	float suma=0;
	float *ret;
	srand(time(NULL));
	pthread_t hebra[N];
	for(int i=0;i<N;i++){
		if( pthread_create(&(hebra[i]), NULL, function, NULL) )
    	{
	  		fprintf(stderr, "Error creating thread\n");
	  		exit(EXIT_FAILURE); 
		}
	}
	for(int i=0; i<N; i++){
		if(pthread_join(hebra[i], (void**) &ret))
		{
		  perror("Fallo en pthread_join()...\n");		
		  exit(EXIT_FAILURE);
		}
		printf("\nSoy la hebra principal");
		printf("\nVALOR RECIBIDO: %0.2f\n", *ret);
		suma= *ret + suma;
	}
	printf("\nSoy la hebra principal");
	printf("\nSUMA TOTAL: %0.2f\n", suma);
	exit(EXIT_SUCCESS);
}

void * function(){
	float x, y;
	float *suma= malloc(sizeof(float));
	printf("\nSoy la hebra %lu", pthread_self());
	x=rand()%100;
	y=rand()%100;
	printf("\nVALOR X: %0.2f, VALOR Y: %0.2f\n", x, y);
	*suma= x+y;
	pthread_exit((void *)suma); 
}