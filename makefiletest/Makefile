
OBJS := lcd.o  key.o main.o

edit: $(OBJS)
	gcc -o edit $(OBJS)

%.d:%.c
	@set -e; rm -f $@
	gcc -MM $< $@.$$$$
# static mode to compile c file
$(OBJS): %.o:%.c
	gcc -g -c $< -o $@
clean:
	rm $(OBJS) edit
