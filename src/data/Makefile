
INC:=-I../../include -I../../include/data

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBDATATYPE:=libdatatype.a

LIBS:=$(LIBDATATYPE)

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
		./text.c		\
		./stree.c	\
		./rb_tree.c	\
		./fib-heap.c	\
		./os_rank.c	\
		./simple_tree.c


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
		./text.o			\
		./stree.o	\
		./rb_tree.o	\
		./fib-heap.o	\
		./os_rank.o	\
		./simple_tree.o


all: $(LIBS)

$(LIBDATATYPE):$(DATA_OBJS)
	$(AR) -r $(LIBDATATYPE) $(DATA_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(DATA_OBJS) $(LIBS)


