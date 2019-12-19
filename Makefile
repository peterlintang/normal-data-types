
INC:=./include

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBDATATYPE:=libdatatype.a


all: libs
	$(GCC) -I$(INC) test.c ./src/$(LIBDATATYPE) 
	echo "done all"
libs:
	make -C src all

clean:
	make -C src clean
	rm -rf a.out
	echo "done clean"


