/*Implemente un programa que cuente las líneas de los ficheros de texto que se le pasen como
parámetros y al final muestre el número de líneas totales (contando las de todos los ficheros juntos).
Ejemplo de llamada: ./a.out fichero1 fichero2 fichero3
Debe crear un hilo por cada fichero indicado por linea de argumentos, de forma que todos los
ficheros se procesen de manera paralela, uno por cada hilo*/
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>

void *function(void *);

int main(int argn, char **argp){
	int N= argn-1, Lineas=0;
	if(N<1){
		fprintf(stderr, "Numero de argumentos invalido\n");
		exit(EXIT_FAILURE);
	}

	pthread_t hebra[N];
	int *ret;

	for(int i=0;i<N;i++){
		if( pthread_create(&(hebra[i]), NULL, function, (void *)argp[i+1]) )
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
		printf("Lineas de %s: %d\n", argp[i+1],*ret);
		Lineas+= *ret;
	}
	printf("\nNUMERO TOTAL LINEAS: %d\n", Lineas);
	exit(EXIT_SUCCESS);
}

void *function(void *arg){
	char *fich= (char *)arg;
	int *lineas=malloc(sizeof(int));
	*lineas=0;
	char s[128];
	printf("Contando fichero %s\n", fich);
	FILE *f =fopen(fich, "r");
	if(f==NULL){
		printf("Error al abrir el fichero %s\n", fich);
		exit(EXIT_FAILURE);
	}
	while(fgets(s, 128, f)){
		*lineas=*lineas+1;
	}
	pthread_exit((void *)lineas); 
}