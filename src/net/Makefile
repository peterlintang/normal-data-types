
INC:=-I../../include -I../../include/net -I../../include/data

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBNET:=libnet.a

LIBS:=$(LIBNET)

NET_SRCS := 	\
		./socket.c				\
		./resolve.c				\
		./route.c				\
		./udp.c					\
		./tcp.c


NET_OBJS := 	\
		./socket.o				\
		./resolve.o				\
		./route.o				\
		./udp.o					\
		./tcp.o


all: $(LIBS)

$(LIBNET):$(NET_OBJS)
	$(AR) -r $(LIBNET) $(NET_OBJS)


%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(NET_OBJS) $(LIBS)


