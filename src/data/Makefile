
INC:=-I../../include -I../../include/data -I../../include/data/aes

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBDATATYPE:=libdatatype.a

LIBS:=$(LIBDATATYPE)

DATA_SRCS := ./ap.c			\
		./arrayrep.c	\
		./ring.c		\
		./ring_buf.c		\
		./set.c			\
		./set-list.c			\
		./table.c		\
		./xp.c			\
		./array.c		\
		./atom.c		\
		./bit.c			\
		./sbit.c			\
		./mp.c			\
		./seq.c			\
		./str.c			\
		./dlist.c		\
		./sentinel-linked-list.c		\
		./list.c		\
		./arena.c		\
		./queue.c		\
		./queue_link.c		\
		./queue_array.c		\
		./stack.c		\
		./stack_link.c		\
		./stack_array.c		\
		./text.c		\
		./rb_tree.c	\
		./b-tree.c	\
		./interval_tree.c	\
		./fib-heap.c	\
		./os_rank.c	\
		./gve.c	\
		./base64.c	\
		./max-heapify.c	\
		./min-heapify.c	\
		./gve_array.c	\
		./aes/block-cipher.c		\
		./aes/gmult.c		\
		./aes/null.c		\
		./aes/cbc.c		\
		./aes/ecb.c		\
		./aes/cfb.c		\
		./aes/ofb.c		\
		./aes/pcbc.c		\
		./aes/aes.c		


DATA_OBJS := ./ap.o			\
		./arrayrep.o	\
		./ring.o		\
		./ring_buf.o		\
		./set.o			\
		./set-list.o			\
		./table.o		\
		./xp.o			\
		./array.o		\
		./atom.o		\
		./bit.o			\
		./sbit.o			\
		./mp.o			\
		./seq.o			\
		./str.o			\
		./dlist.o		\
		./sentinel-linked-list.o		\
		./list.o		\
		./arena.o		\
		./queue.o		\
		./queue_link.o		\
		./queue_array.o		\
		./stack.o		\
		./stack_link.o		\
		./stack_array.o		\
		./text.o			\
		./rb_tree.o	\
		./b-tree.o	\
		./interval_tree.o	\
		./fib-heap.o	\
		./os_rank.o	\
		./gve.o	\
		./base64.o	\
		./max-heapify.o	\
		./min-heapify.o	\
		./gve_array.o	\
		./aes/block-cipher.o		\
		./aes/gmult.o		\
		./aes/null.o		\
		./aes/cbc.o		\
		./aes/ecb.o		\
		./aes/cfb.o		\
		./aes/ofb.o		\
		./aes/pcbc.o		\
		./aes/aes.o		


all: $(LIBS)

$(LIBDATATYPE):$(DATA_OBJS)
	$(AR) -r $(LIBDATATYPE) $(DATA_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(DATA_OBJS) $(LIBS)


