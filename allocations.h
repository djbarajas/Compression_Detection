#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

char * 
allocate_strmem(int len);

int * 
allocate_intmem(int len);

uint8_t *
allocate_ustrmem(int len);



#endif