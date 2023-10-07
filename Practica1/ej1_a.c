#include <sys/types.h> //Para tipo pid_t 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Para fork()
#include <errno.h>
#include <sys/wait.h>
#include <string.h> 

int main(int argc, char **argv) 
{
    pid_t pid, childpid;
    int status;

    if(argc!=2)
    {
    	printf("\nError, la entrada de argumentos no se ha hecho correctamente\n\n");
    	exit(EXIT_FAILURE);

    }
    int n=atoi(argv[1]);        

    for(int i=0;i<n;i++)
    {
    	pid = fork(); //Creamos un proceso
    	//El sistema devuelve al padre el PID del hijo creado, y al hijo le devuelve un valor 0
    	if (pid == -1) //Entra si hay un error al crear el proceso
    	{
      		perror("fork error");
      		printf("errno value= %d\n", errno); //Tipo de error
      		exit(EXIT_FAILURE);
    	}
    	else if (pid == 0) //hijo
    	{
    		printf("Soy el Hijo %d, mi PID es %d y el PPID de mi padre es %d \n", i+1, getpid(), getppid()); 
			exit(EXIT_SUCCESS);//Acabamos el proceso ya que no queremos que un hijo cree otro
    	}
    	else /* padre */ 
    	{
      		if(i==0)
      		{
      			printf("Soy el Padre, mi PID es %d , el PPID de mi padre es %d\n", getpid(), getppid());
													/*Averigua quien es el padre del proceso padre*/
      		}
		}
	}
	//Se espera al hijo
	//Uso de wait(). No permite detectar la parada y reanudacion de procesos hijos: WIFSTOPPED(status), WIFCONTINUED(status)
	while ( (childpid=wait(&status)) > 0 ) 
	{
		if (WIFEXITED(status)) 
		{
			printf("Proceso padre %d, hijo con PID %ld finalizado, status = %d\n", getpid(), (long int)childpid, WEXITSTATUS(status));
		} 
		else if (WIFSIGNALED(status)) //Para seniales como las de finalizar o matar
		{
			printf("Proceso padre %d, hijo con PID %ld finalizado al recibir la señal %d\n", getpid(), (long int)childpid, WTERMSIG(status));
		} 
	}

	if (childpid==(pid_t)-1 && errno==ECHILD) //Entra cuando vuelve al while y no hay más hijos que esperar
	{
		printf("Proceso padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
	}	
	else
	{
		printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	//No salimos del programa ya que queremos que el mismo padre tenga diferentes hijos
}