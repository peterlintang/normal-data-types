
INC:=../include

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBDATATYPE:=libdatatype.a
LIBCOMMUNICATE:=libcommunicate.a

LIBS:=$(LIBDATATYPE) $(LIBCOMMUNICATE)

DATA_SRCS := ./ap.c			\
		./arrayrep.c	\
		./ring.c		\
		./set.c			\
		./table.c		\
		./xp.c			\
		./array.c		\
		./bit.c			\
		./mp.c			\
		./seq.c			\
		./str.c			\
		./dlist.c		\
		./arena.c		\
		./thread_pool.c		\
		./queue_link.c		\
		./stack_link.c		\
		./text.c	

COMMUNICATE_SRCS := ./udsock-server.c \
		./udsock-client.c	\
		./thread-signal.c	\
		./named-sem.c	\
		./share-mem.c	\
		./sem-sysv.c	\
		./mq-process.c


DATA_OBJS := ./ap.o			\
		./arrayrep.o	\
		./ring.o		\
		./set.o			\
		./table.o		\
		./xp.o			\
		./array.o		\
		./bit.o			\
		./mp.o			\
		./seq.o			\
		./str.o			\
		./dlist.o		\
		./arena.o		\
		./thread_pool.o		\
		./queue_link.o		\
		./stack_link.o		\
		./text.o	

COMMUNICATE_OBJS := ./udsock-server.o \
		./udsock-client.o	\
		./thread-signal.o	\
		./named-sem.o	\
		./share-mem.o	\
		./sem-sysv.o	\
		./mq-process.o

all: $(LIBS)

$(LIBDATATYPE):$(DATA_OBJS)
	$(AR) -r $(LIBDATATYPE) $(DATA_OBJS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) -I$(INC) -g -c $< -o $@

clean:
	rm -rf $(DATA_OBJS) $(COMMUNICATE_OBJS) $(LIBS)


