 PROGS = client_cooperative server_cooperative standalone read_json allocations packet_setup t
 OBJS = read_json.o allocations.o packet_setup.o signal.o

 CFLAGS = -g 

standalone : standalone.c read_json.o allocations.o packet_setup.o signal.o
	gcc ${CFLAGS} -o $@ $^

client_cooperative : client_cooperative.c read_json.o allocations.o packet_setup.o signal.o
	gcc ${CFLAGS} -o $@ $^

server_cooperative : server_cooperative.c read_json.o allocations.o packet_setup.o signal.o
	gcc ${CFLAGS} -o $@ $^

read_json : read_json.c read_json.h read_json.o
	gcc ${CFLAGS} -o $@ $^

allocations : allocations.c allocations.h allocations.o
	gcc ${CFLAGS} -o $@ $^

packet_setup : packet_setup.c packet_setup.h packet_setup.o
	gcc ${CFLAGS} -o $@ $^

signal : signal.c signal.h signal.o
	gcc ${CFLAGS} -o $@ $^

t: t.c
	gcc ${CFLAGS} -o $@ $^ -lpcap

clean:
	rm -rf ${PROGS} ${OBJS} *.dSYM

%.o : %.c %.h