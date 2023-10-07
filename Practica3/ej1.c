/*Una tienda que vende camisetas guarda en una base de datos (buffer) las cantidades de
camisetas que tiene según el modelo. Por ejemplo, un buffer de camisetas[5] indica que existen 5
modelos de camisetas y cada elemento de este buffer guarda las cantidades iniciales de cada una de
ellas.
Implementar un programa que genere N clientes (pedido por linea de argumentos) y M
proveedores (la misma cantidad de proveedores que modelos de camiseta, también pedido por linea
de argumentos).
Para simular una compra, cada hilo Cliente debe generar un valor aleatorio para el modelo de
camiseta que quiere comprar y otro para la cantidad a comprar. Esta cantidad debe decrementar el
stock de la camiseta en cuestión.
Para simular un suministro, cada Proveedor debe hacer lo mismo que el Cliente pero en este
caso, incrementando el stock de la camiseta en cuestión.
Utilice semáforos binarios para resolver este problema de concurrencia imprimiendo el buffer
antes de generar los hilos y al final del programa para comprobar que se ha ejecutado
correctamente.
En cuanto al numero de camisetas a comprar o suministrar, por ejemplo pueden ser valores
aleatorios entre [1-10]. Para rellenar el vector inicial de stock de camisetas podría generar números
aleatorios entre [1-100]. Si se diera el caso de que algún cliente quiere comprar más camisetas de
las disponibles solo se le proporcionarán las que queden.
El ejercicio se puede implementar de diversas maneras. La salida que se muestra a
continuación como ejemplo ha hecho uso en la zona de variables globales de un puntero a un vector
de enteros (para el vector de camisetas) y de la creación e inicialización de un mutex*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int *camisetas;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; //Mutex

void *comprar(void *N);
void *suministrar(void *N);

int main(int argc, char *argv[]){

	if(argc!=3){
		printf("Numero de argumentos invalido\n");
		exit(EXIT_FAILURE);
	}

	pthread_t *clientes, *proveedores;
	int N;
	int M;
	N=atoi(argv[1]);
	M=atoi(argv[2]);

	camisetas= (int *) malloc ((M)*sizeof(int));
	clientes= (pthread_t *) malloc ((N)*sizeof(pthread_t));
	proveedores= (pthread_t *) malloc ((M)* sizeof(pthread_t));

	srand(time(NULL));
	printf("Stock de %d tipos de camisetas al abrir la tienda\n", M);
	for(int i=0;i<M;i++){
		camisetas[i]=rand()%101;
		printf("[%d]:%d  ", i, camisetas[i]);
	}
	printf("\n\n");

	for(int i=0;i<N;i++){
		if(pthread_create(&(clientes[i]), NULL, (void *)comprar, (void *)&M)){
			printf("Error creating thread\n");
			exit(EXIT_FAILURE);
		}
	}

	for(int i=0;i<M;i++){
		if(pthread_create(&(proveedores[i]), NULL, (void *)suministrar, (void *)&M)){
			printf("Error creating thread\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("MAIN - Esperando a las hebras clientes y proveedores creadas...\n");
	for(int i=0;i<N;i++){
		if(pthread_join(clientes[i], NULL)){
			printf("Error joining thread\n");
			exit(EXIT_FAILURE);
		}
	}

	for(int i=0;i<M;i++){
		if(pthread_join(proveedores[i], NULL)){
			printf("Error joining thread\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("\nStock de %d tipos de camisetas al cerrar la tienda\n", M);
	for(int i=0;i<M;i++){
		printf("[%d]:%d  ", i, camisetas[i]);
	}
	printf("\n");
}

void *comprar(void *M){
	int s, n;
	int *X= (int *)M;
	n= rand()%*X;
	int cant;
	cant= (rand()%10)+1;

	s= pthread_mutex_lock(&mtx);
	if(s!=0){
		printf("mutex lock error...\n");
		pthread_exit(NULL);
	}
	if(cant>camisetas[n]){
		cant= camisetas[n];
	}
	camisetas[n]-=cant;
	s= pthread_mutex_unlock(&mtx);
	if(s!=0){
		printf("mutex unlock error...\n");
		pthread_exit(NULL);
	}

	printf("<--Cliente %lu, compro %d unidades de la camiseta %d\n", pthread_self(), cant, n);

	pthread_exit(NULL);
}

void *suministrar(void *M){
	int s, n;
	int *X= (int *)M;
	n= rand()%*X;
	int cant= (rand()%10)+1;

	s= pthread_mutex_lock(&mtx);
	if(s!=0){
		printf("mutex lock error...\n");
		pthread_exit(NULL);
	}
	camisetas[n]+= cant;
	s= pthread_mutex_unlock(&mtx);
	if(s!=0){
		printf("mutex unlock error...\n");
		pthread_exit(NULL);
	}

	printf("-->Proveedor %lu, suministro %d unidades de la camiseta %d\n", pthread_self(), cant, n);

	pthread_exit(NULL);
}