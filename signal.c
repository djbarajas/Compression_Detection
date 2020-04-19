#include "signal.h"

/**
 * @file signal.c
 *
 * source code exploited by our clients and server to shut down hosts that hold delays
 */

/**
 * signal handler function
 *
 * @param signo | this is the type of signal returned to process error
 *
 * Does not envoke any local helper functions
 */
void 
sigalarm_handler(int signo)
{
	printf("Program ran too long. Please wait a bit before rerunning. Shutting down...\n");
	exit(EXIT_FAILURE);
}