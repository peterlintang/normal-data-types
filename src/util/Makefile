
INC:=-I../../include -I../../include/data -I../../include/util

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBCOMMUNICATE:=libutil.a

LIBS:=$(LIBCOMMUNICATE)

COMMUNICATE_SRCS := ./components.c 	\
					./gve-normal.c	\
					./err.c	\
					./log.c	\
					./minmax.c	\
					./quark.c	\
					./quicksort.c	\
					./insert-sort.c	\
					./merge-sort.c	\
					./randomized-select.c	\
					./max-heapify-sort.c	\
					./thread_pool.c

COMMUNICATE_OBJS := ./components.o 	\
					./gve-normal.o	\
					./err.o	\
					./log.o	\
					./minmax.o	\
					./quark.o	\
					./quicksort.o	\
					./insert-sort.o	\
					./merge-sort.o	\
					./randomized-select.o	\
					./max-heapify-sort.o	\
					./thread_pool.o

all: $(LIBS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(COMMUNICATE_OBJS) $(LIBS)


