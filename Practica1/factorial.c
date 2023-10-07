#include <stdio.h>
#include <stdlib.h>

int main(int argv, char **argc)
{
	int n=atoi(argc[1]);
	int fact=1;

	for(int i=n; i>1; i--)
	{
		fact=fact*i;
	}
printf("Factorial de %i = %i\n", n, fact);
exit(EXIT_SUCCESS);
	
}