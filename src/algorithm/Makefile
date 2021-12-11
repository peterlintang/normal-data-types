
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
					./ve.c \
					./dfs.c \
					./bfs.c \
					./mst-kruskal.c \
					./mst-prim.c \
					./bellman-ford.c \
					./dag-shortest-paths.c \
					./topological-sort.c \
					./randomized-select.c \
					./max-heapify-sort.c \
					./min-heapify-sort.c \
					./counting-sort.c \
					./components.c 

COMMUNICATE_OBJS := \
					./gve-normal.o \
					./insert-sort.o \
					./merge-sort.o \
					./quicksort.o \
					./minmax.o \
					./ve.o \
					./dfs.o \
					./bfs.o \
					./mst-kruskal.o \
					./mst-prim.o \
					./bellman-ford.o \
					./dag-shortest-paths.o \
					./topological-sort.o \
					./randomized-select.o \
					./max-heapify-sort.o \
					./min-heapify-sort.o \
					./counting-sort.o \
					./components.o 

all: $(LIBS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(COMMUNICATE_OBJS) $(LIBS)


