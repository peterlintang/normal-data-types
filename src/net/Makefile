
INC:=-I../../include -I../../include/net

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBNET:=libnet.a

LIBS:=$(LIBNET)

NET_SRCS := ./client_addr.c			\
		./client.c					\
		./server_addr.c				\
		./server.c			


NET_OBJS := ./client_addr.o			\
		./client.o					\
		./server_addr.o			\
		./server.o


all: $(LIBS)

$(LIBNET):$(NET_OBJS)
	$(AR) -r $(LIBNET) $(NET_OBJS)


%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(NET_OBJS) $(LIBS)


