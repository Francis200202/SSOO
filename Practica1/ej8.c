#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv)
{

pid_t pid=atoi(argv[1]);

kill(pid, SIGUSR1);

sleep(10);

kill(pid, SIGKILL);

}