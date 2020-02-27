
INC:=-I../../include -I../../include/inifile

GCC:=gcc

FLAGS:=-c

AR:=ar

LIBINIFILE:=libinifile.a

LIBS:=$(LIBINIFILE)

INIFILE_SRCS := ./configuration.c			\
		./inifile.c					\
		./parameter.c				\
		./str.c						\
		./file.c					\
		./parse_params.c			\
		./category.c			


INIFILE_OBJS := ./configuration.o			\
		./inifile.o					\
		./parameter.o				\
		./str.o						\
		./file.o					\
		./parse_params.o			\
		./category.o


all: $(LIBS)

$(LIBINIFILE):$(INIFILE_OBJS)
	$(AR) -r $(LIBINIFILE) $(INIFILE_OBJS)


%.o:%.c
	$(GCC) $(INC) -g -c $< -o $@

clean:
	rm -rf $(INIFILE_OBJS) $(LIBS)


