#include "signal.h"


void 
sigalarm_handler(int signo)
{
	printf("Program ran too long.  Please wait a bit before rerunning. Shutting down...\n");
	exit(EXIT_FAILURE);
}