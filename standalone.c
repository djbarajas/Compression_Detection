#include <stdio.h>
#include "read_json.h"

int main(){
	struct json packet_info;
	
	read_json(&packet_info, "myconfig.json", buff);
	
	return 0;
}
