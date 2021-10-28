
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
					./quark.c	\
					./quicksort.c	\
					./insert-sort.c	\
					./thread_pool.c

COMMUNICATE_OBJS := ./components.o 	\
					./gve-normal.o	\
					./err.o	\
					./log.o	\
					./quark.o	\
					./quicksort.o	\
					./insert-sort.o	\
					./thread_pool.o

all: $(LIBS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(COMMUNICATE_OBJS) $(LIBS)


