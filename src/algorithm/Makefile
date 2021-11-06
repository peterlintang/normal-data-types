
INC:=-I../../include -I../../include/data -I../../include/algorithm

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBCOMMUNICATE:=libalgorithm.a

LIBS:=$(LIBCOMMUNICATE)

COMMUNICATE_SRCS := \
					./gve-normal.c \
					./insert-sort.c \
					./merge-sort.c \
					./quicksort.c \
					./minmax.c \
					./randomized-select.c \
					./max-heapify-sort.c \
					./min-heapify-sort.c \
					./components.c 

COMMUNICATE_OBJS := \
					./gve-normal.o \
					./insert-sort.o \
					./merge-sort.o \
					./quicksort.o \
					./minmax.o \
					./randomized-select.o \
					./max-heapify-sort.o \
					./min-heapify-sort.o \
					./components.o 

all: $(LIBS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(COMMUNICATE_OBJS) $(LIBS)


