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
    struct Bulk b;
    int len;
    int max = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    
    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    memset(&b, 0, sizeof(struct Bulk));
    b.sequence = -1;
    for(;;){
        len = recvfrom(sockfd, &b, sizeof(struct Bulk), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        printf("Sequence num: %d\n", b.sequence);
        if(b.sequence > max) 
            max = b.sequence;
        else
            printf("Error!\n");
    }
    return 0;
}
