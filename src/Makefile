
INC:=../include

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBDATATYPE:=libdatatype.a

LIBS:=$(LIBDATATYPE)

SRC := ./ap.c			\
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
		./queue_link.c		\
		./stack_link.c		\
		./text.c	

OBJS := ./ap.o			\
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
		./queue_link.o		\
		./stack_link.o		\
		./text.o	


all: $(LIBS)
	echo "done all"

$(LIBDATATYPE):$(OBJS)
	$(AR) -r $(LIBDATATYPE) $(OBJS)
	echo "done libs"

%.o:%.c
	$(GCC) -I$(INC) -c $< -o $@

clean:
	rm -rf $(OBJS)  $(LIBS)


