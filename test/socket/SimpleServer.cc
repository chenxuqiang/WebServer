#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "Student.h"

#define PORT 4321
#define BUFFER_SIZE 1024
#define MAX_QUE_CONN_NM 5

int main(int argc, char *argv[])
{
    struct sockaddr_in serverSockAddr, clientSockAddr;
    socklen_t sinSize;
    int recvBytes, sendBytes;
    int sockFd, clientFd;

    char buf[BUFFER_SIZE];
    struct StudentTable *table = InitStudentTable();
    if (table == NULL) {
        fprintf(stderr, "Init StudentTable failed.\n");
        exit(1);
    }

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(PORT);
    serverSockAddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serverSockAddr.sin_zero), 8);

    if (bind(sockFd, (struct sockaddr *)&serverSockAddr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    fprintf(stdout, "Bind Success.\n");

    if (listen(sockFd, MAX_QUE_CONN_NM) == -1) {
        perror("listen");
        exit(1);
    }

    fprintf(stdout, "Listening...\n");
    sinSize = sizeof(clientSockAddr);

    if ((clientFd = accept(sockFd, (struct sockaddr *)&clientSockAddr, &sinSize)) == -1) {
        perror("accept");
        exit(1);
    }

    while (true) {
        struct QueryRequest req;
        if ((recvBytes = recv(clientFd, &req, sizeof(req), 0)) == -1) {
            perror("recv");
            exit(1);
        }
        fprintf(stdout, "Recvied a queryRequest: id = %d\n", req.stuId);

        QueryStudentTable(table, &req);

        if ((sendBytes = send(clientFd, &req, sizeof(req), 0)) == -1) {
            perror("send");
            exit(1);
        }
        fprintf(stdout, "Send a response.\n");
    }

    close(sockFd);
    return 0;
}

