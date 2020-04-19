#include "allocations.h"

/**
 * @file allocations.c
 *
 * source code exploited by our clients and server for error-free 
   allocation of memory through size dependencies.
 *
 */



/**
 * integer array memory allocation function
 *
 * this function is responsible for setting fixed space for an integer array
 *
 * @param len | this is the size of the array (number of elements)
 *
 * Does not envoke any local helper functions
 */
char * 
allocate_strmem(int len)
{
	void *tmp;

	if (len <= 0) {
		fprintf(stderr, "ERROR: Cannot allocate memory because len = %i in allocate_strmem().\n", len);
		exit(EXIT_FAILURE);
	}

	tmp = (char *)malloc(len * sizeof (char));
	if (tmp != NULL){
		memset(tmp, 0, len * sizeof (char));
		return(tmp);
	} else {
		fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_strmem().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * signed char array memory allocation function
 *
 * this function is responsible for setting fixed space for an signed char array
 *
 * @param len | this is the size of the array (number of elements)
 *
 * Does not envoke any local helper functions
 */
int * 
allocate_intmem(int len)
{
	void *tmp;

	if (len <= 0) {
		fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_intmem().\n",len);
		exit(EXIT_FAILURE);
	}

	tmp = (int *)malloc(len * sizeof(int));
	if(tmp != NULL){
		memset (tmp, 0, len * sizeof(int));
		return (tmp);
	} else{
		fprintf(stderr,"ERROR: Cannot allocate memory for array allocate_intmem().\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * unsigned char array memory allocation function
 *
 * this function is responsible for setting fixed space for an unsigned char array
 *
 * @param len | this is the size of the array (number of elements)
 *
 * Does not envoke any local helper functions
 */
uint8_t *
allocate_ustrmem(int len)
{
	void *tmp;

	if (len <= 0) {
		fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_ustrmem().\n", len);
		exit (EXIT_FAILURE);
	}

	tmp = (uint8_t *) malloc (len * sizeof (uint8_t));
	if (tmp != NULL) {
		memset (tmp, 0, len * sizeof (uint8_t));
		return (tmp);
	} else {
		fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_ustrmem().\n");
		exit (EXIT_FAILURE);
	}
}