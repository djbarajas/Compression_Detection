#ifndef _PKT_STP_H_
#define _PKT_STP_H_

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>


void packet_id_setup (char* bin, unsigned int val);
void read_high_entropy_data(uint8_t * data, int len);

#endif