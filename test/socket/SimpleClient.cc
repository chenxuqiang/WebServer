#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "Student.h"

#define PORT 4321
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sockFd, sendBytes, recvBytes;
    struct hostent *host;
    struct sockaddr_in serverAddr;

    if (argc < 2) {
        fprintf(stdout, "Usage: %s <Hostname(or ip address)>\n", argv[0]);
        exit(1);
    }

    if ((host = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(PORT);
    serverAddr.sin_addr   = *((struct in_addr *)host->h_addr);
    bzero(&(serverAddr.sin_zero), 8);

    if (connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    while (true) {
        fprintf(stdout, "input Student ID: ");
        int input = 0;
        scanf("%d", &input);
        if (input == 0) {
            break;
        }

        struct QueryRequest req;
        req.stuId = input;
        req.rep.rval = 0;

        if ((sendBytes = send(sockFd, &req, sizeof(req), 0)) == -1) {
            perror("send");
            exit(1);
        }

        memset(&req, 0, sizeof(req));

        if ((recvBytes = recv(sockFd, &req, sizeof(req), 0)) == -1) {
            perror("recv");
            exit(1);
        }

        if (req.rep.rval == 1) {
            fprintf(stdout, "Recived a response: student exist, name is %s.\n", req.rep.name);
        } else {
            fprintf(stdout, "Recived a response: student not exist.\n");
        }
    }

    close(sockFd);
    return 0;
}