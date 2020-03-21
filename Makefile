 PROGS = client_cooperative server_cooperative standalone
 OBJS = read_json.o 

 CFLAGS = -g

client_cooperative : client_cooperative.c read_json.o
	gcc ${CFLAGS} -o $@ $^

server_cooperative : server_cooperative.c read_json.o
	gcc ${CFLAGS} -o $@ $^

clean:
	rm -rf ${PROGS} ${OBJS}

%.o : %.c %.h