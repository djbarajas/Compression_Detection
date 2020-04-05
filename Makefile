 PROGS = client_cooperative server_cooperative standalone read_json
 OBJS = read_json.o 

 CFLAGS = -g

standalone : standalone.c read_json.o

client_cooperative : client_cooperative.c read_json.o
	gcc ${CFLAGS} -o $@ $^

server_cooperative : server_cooperative.c read_json.o
	gcc ${CFLAGS} -o $@ $^

read_json : read_json.c read_json.h read_json.o
	gcc ${CFLAGS} -o $@ $^


clean:
	rm -rf ${PROGS} ${OBJS} *.dSYM

%.o : %.c %.h