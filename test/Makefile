
#	gcc -g dfs.c -o dfs -I../include -I../include/data/ ../src/data/libdatatype.a
#	gcc -g process-lock-test.c -o process-lock-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
all:
	gcc -g mq-test.c -o mq-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g name_fifo.c -o fifo-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g sem_test.c -o sem-test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g sem_sysv_test.c -o sem-sysv_test -I../include -I../include/pic ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g components_main.c -o components -I../include -I../include/util -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/util/libutil.a ../src/data/libdatatype.a -lrt -lpthread
	gcc -g quicksort-test.c -o quicksort-test -I../include -I../include/util -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/util/libutil.a ../src/data/libdatatype.a -lrt -lpthread
	gcc -g share-mem-test.c -o share-mem-test -I../include -I../include/pic -I../include/data ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g share-mem-test-read.c -o share-mem-test-read -I../include -I../include/pic -I../include/data ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g ring_buf_test.c -o ring_buf_test -I../include -I../include/pic -I../include/data ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g ring_buf_sharemem_test.c -o ring_buf_sharemem_test -I../include -I../include/pic -I../include/data ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g dfs-test.c -o dfs-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g bfs-test.c -o bfs-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g topological-sort-test.c -o topological-sort-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g strongly-components-test.c -o strongly-components-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g generic-mst-test.c -o generic-mst-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g min-heapify-sort-test.c -o min-heapify-sort-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g max-heapify-sort-test.c -o max-heapify-sort-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g mst-prim-test.c -o mst-prim-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g bellman-ford-test.c -o bellman-ford-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g dag-shortest-paths-test.c -o dag-shortest-paths-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g dijkstra-test.c -o dijkstra-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g ud-client-test.c -o ud-client-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
	gcc -g ud-server-test.c -o ud-server-test -I../include -I../include/pic -I../include/data -I../include/algorithm ../src/algorithm/libalgorithm.a ../src/data/libdatatype.a ../src/pic/libcommunicate.a -lrt -lpthread
clean:
