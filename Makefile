 PROGS = client_cooperative server_cooperative standalone read_json allocations packet_setup
 OBJS = read_json.o allocations.o packet_setup.o signal.o

 CFLAGS = -g 

standalone : standalone.c read_json.o allocations.o packet_setup.o signal.o
	gcc ${CFLAGS} -o $@ $^ -lpcap

client_cooperative : client_cooperative.c read_json.o allocations.o packet_setup.o signal.o
	gcc ${CFLAGS} -o $@ $^

server_cooperative : server_cooperative.c read_json.o allocations.o packet_setup.o signal.o
	gcc ${CFLAGS} -o $@ $^

clean:
	rm -rf ${PROGS} ${OBJS} *.dSYM

%.o : %.c %.h