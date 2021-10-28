
INC:=-I../../include -I../../include/pic

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBCOMMUNICATE:=libcommunicate.a

LIBS:=$(LIBCOMMUNICATE)

COMMUNICATE_SRCS := ./udsock-server.c \
		./udsock-client.c	\
		./thread-signal.c	\
		./named-sem.c	\
		./share-mem.c	\
		./sem-sysv.c	\
		./tellwaitp.c	\
		./tell_wait_sig.c	\
		./mq-process.c	\
		./process-lock.c	\
		./named-fifo.c


COMMUNICATE_OBJS := ./udsock-server.o \
		./udsock-client.o	\
		./thread-signal.o	\
		./named-sem.o	\
		./share-mem.o	\
		./sem-sysv.o	\
		./tellwaitp.o	\
		./tell_wait_sig.o	\
		./mq-process.o	\
		./process-lock.o	\
		./named-fifo.o

all: $(LIBS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(COMMUNICATE_OBJS) $(LIBS)


