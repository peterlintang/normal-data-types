
INC:=-I./include -I./include/data

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBDATATYPE:=./src/data/libdatatype.a


all: libs
	$(GCC) $(INC) -g test.c $(LIBDATATYPE) -lpthread
	echo "done all"
libs:
	make -C src/ all

clean:
	make -C src/ clean
	rm -rf a.out
	echo "done clean"


