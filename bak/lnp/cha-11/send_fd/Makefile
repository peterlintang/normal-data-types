
all: server openfile

server:
	gcc -o server send_fd_server.c send_fd_common.c
openfile:
	gcc -o openfile send_fd_client.c send_fd_common.c


clean:
	rm server openfile
