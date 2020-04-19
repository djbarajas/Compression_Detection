/**
* @file signal.h
*
* header file for handling delays caused by internal errors
*/


#ifndef _SIG_H_
#define _SIG_H_

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


/* Function prototypes */
void 
sigalarm_handler(int signo);



#endif