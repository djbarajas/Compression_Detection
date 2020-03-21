
#include "read_json.h"

void read_json (struct json * to_read, char * file_name)
{
	FILE* file_ptr = NULL;
	file_ptr =  fopen(file_name, "r");
	char *buffer;
    size_t bufsize = 100;
    size_t characters;
    while (true)
    {
	    buffer = (char *) malloc(bufsize * sizeof(char));
	    if (buffer == NULL)
	    {
	    	continue;
	    }
	    characters = getline(&buffer,&bufsize,file_ptr);
    	if (feof(file_ptr))
    	{
    		free(buffer);
    		break;
    	}
    	printf("%s\n", buffer);
    	free(buffer);
    }

    fclose(file_ptr);
}

int main(int argc, char ** argv)
{
	struct json to_read;
	read_json(&to_read, "myconfig.json");
}