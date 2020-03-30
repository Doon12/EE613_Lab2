#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define GEN 0
#define LIST 1
#define END 2

void error(char* msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]) {
	int sockfd;
	struct sockaddr_in servaddr;

	// (g) socket()
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket create\n");
	printf("socket file descriptor : %d\n", sockfd);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	// (h) connect()
	int connectResult = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("connect Result %d\n", connectResult);

	while(1) {
		int send_msg, len;
		char recv_msg[256];
		printf("Command (0:GEN 1:LIST 2:END): ");
		scanf("%d", &send_msg);
		// (i) write()
		int writeLen = write(sockfd, &send_msg, sizeof(send_msg));
		if(send_msg == END)
			break;			
		// (j) read()
		int readLen = read(sockfd, recv_msg, sizeof(recv_msg));
		len = readLen;
		recv_msg[len] = 0;
		printf("%s\n", recv_msg);
	}
	close(sockfd);
	return 0;
}
