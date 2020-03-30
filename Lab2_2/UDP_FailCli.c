#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 255

void error_handling(char* msg) {
    perror(msg);
    exit(1);
}

struct Bulk {
    int sequence;
    char pad[MAX];
};

int main(int argc, char **argv)
{
    int sockfd;
    int str_len;
    struct sockaddr_in serv_addr;
    socklen_t serverlen;
    struct Bulk b;
    int i;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
        error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    memset(&b, 0, sizeof(struct Bulk));
    for(i = 0; ; ++i) {
        b.sequence = i;
        sendto(sockfd, &b, sizeof(struct Bulk), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    }

    close(sockfd);
    return 0;
}
