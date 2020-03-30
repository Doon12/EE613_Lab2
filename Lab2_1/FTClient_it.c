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
	int sockfd, filefd, len;
	struct sockaddr_in servaddr;
	file_segment segment;

	// new
	int testNum;
	char file_path[BUFSIZE];
	char new_file[BUFSIZE];

	printf("Enter the test case number:");
	scanf("%d", &testNum);


	// (h) socket()
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket create\n");
	printf("socket file descriptor : %d\n", sockfd);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	// (i) connect()
	int connectResult = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("connect Result %d\n", connectResult);

	while (testNum--) {
		printf("%d test left\n", testNum);
		printf("Enter the file name:");
		scanf("%s", file_path);
		printf("Enter the new file name:");
		scanf("%s", new_file);

		if((filefd = open(new_file, O_CREAT|O_WRONLY, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)) == -1)
		  error("file open() error");

		// (j) write file path(argv[3]) to server
		int writeLen = write(sockfd, file_path, strlen(file_path));
		if (len = read(sockfd, &segment, sizeof(segment))) { // (k) read segment from server
			// (l) write data to file
			write(filefd, segment.buf, segment.len);
		}
		close(filefd);
	}
	// write termination msg to server
	write(sockfd, TERMMSG, strlen(TERMMSG));

	close(sockfd);
	return 0;
}

