
INC:=-I../../include -I../../include/net

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBNET:=libnet.a

LIBS:=$(LIBNET)

NET_SRCS := 	\
		./socket.c				\
		./resolve.c				\
		./udp.c


NET_OBJS := 	\
		./socket.o				\
		./resolve.o				\
		./udp.o


all: $(LIBS)

$(LIBNET):$(NET_OBJS)
	$(AR) -r $(LIBNET) $(NET_OBJS)


%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(NET_OBJS) $(LIBS)


