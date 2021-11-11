
INC:=-I../../include -I../../include/data -I../../include/util

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBCOMMUNICATE:=libutil.a

LIBS:=$(LIBCOMMUNICATE)

COMMUNICATE_SRCS := ./db.c \
					./netutil.c	\
					./err.c	\
					./log.c	\
					./quark.c	\
					./thread_pool.c

COMMUNICATE_OBJS := ./db.o \
					./netutil.o	\
					./err.o	\
					./log.o	\
					./quark.o	\
					./thread_pool.o

all: $(LIBS)

$(LIBCOMMUNICATE):$(COMMUNICATE_OBJS)
	$(AR) -r $(LIBCOMMUNICATE) $(COMMUNICATE_OBJS)

%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(COMMUNICATE_OBJS) $(LIBS)


