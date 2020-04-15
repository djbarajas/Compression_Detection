#include "packet_setup.h"

/*
* Packet ids
*/
void packet_id_setup (char* bin, unsigned int val)
{
    unsigned int copy_of_value = val;
    for(int i = 15; i >= 0 ;i--)
    {
        bin[i] = (copy_of_value & 0b1) +'0';
        copy_of_value >>= 1;
    }
}


void read_high_entropy_data(uint8_t * data, int len) {
    FILE* file_ptr = NULL;
    file_ptr =  fopen("/dev/random", "r");
    for (int i = 0; i < len-1; i++) {
        data[i] = getc(file_ptr);
    }
    fclose(file_ptr);
}