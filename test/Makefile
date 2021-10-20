
all:
	gcc -g dfs.c -o dfs -I../include -I../include/data/ ../src/data/libdatatype.a
	gcc -g mq-test.c -o mq-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g name_fifo.c -o fifo-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g sem_test.c -o sem-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g sem_sysv_test.c -o sem-sysv_test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
clean:
