#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 256
#define TERMMSG	"TERMINATE CONNECTION"
typedef struct file_segment {
	int len;
	char buf[BUFSIZE];
} file_segment;

void error(char* msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]) {
	int listenfd, connfd, filefd, len;
	struct sockaddr_in cliaddr, servaddr;
	socklen_t clilen;
	char recv_msg[BUFSIZE];
	file_segment segment;

	// (a) socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket create\n");
	printf("socket file descriptor : %d\n", listenfd);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	// (b) bind()
	int bindResult = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("bindResult is %d\n", bindResult);
	if (bindResult < 0) {
		error("bind failed on server");
	}

	// (c) listen()
	int listenResult = listen(listenfd, 5);
	printf("listenResult is %d\n", listenResult);
	if (listenResult < 0) {
		error("listen failed on server");
	}

	// (d) accept()
	clilen = (socklen_t) sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
	printf("accept\n");
	printf("connection file descriptor : %d\n", connfd);

	// (e)
	while (1) {
		len = read(connfd, &recv_msg, sizeof(recv_msg));
		recv_msg[len] = 0;
		if(strcmp(recv_msg, TERMMSG) == 0) {
			printf("exit program...\n");
	        break;
		}

		if((filefd = open(recv_msg, O_RDONLY)) == -1) {
			close(connfd);
			close(listenfd);
			error("file open() error");
		}

		char buf[BUFSIZE];
		while(len = read(filefd, buf, BUFSIZE)) { // (f) read data from file
			segment.len = len;
			strncpy(segment.buf, buf, BUFSIZE);
			// (g) write segment to client
			int writeLength = write(connfd, &segment, sizeof(segment));
		}
		close(filefd);
	}

	close(connfd);
	close(listenfd);
	return 0;
}
