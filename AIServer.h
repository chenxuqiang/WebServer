#ifndef AI_SERVER_H
#define AI_SERVER_H

#include <stdio.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <vector>

#include "ThreadPool.h"
#include "Request.h"

class AIServer
{
public:
    AIServer(int port_): port(prot_) {}
    ~AIServer() { }
    int Run();
    int Init();
    int Stop();
private:
    void addfd(int fd);

    int clientFd;
    int listener;
    int epfd;
    char serverIP[16];
    int port;
    struct sockaddr_in sockAddr;
    struct sockaddr_in clientAddr;
};

void AIServer::addfd(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, f_GETFD, 0) | O_NONBLOCK);

    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    std::cout << "[debug]: " << fd << " add to epoll instance." << std::endl;
}

void AITaskRun(void *args)
{

}

void BufferToTask(void *buffer, )

int AIServer::Run()
{
    ThreadPool tPool = ThreadPool(threadPoolSize);
    int ret = 0;
    int sfd;
    int epollEventsCount = 0;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(serverIP);
    sockAddr.sin_port = htons(prot);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        std::cerr << "[error]: create socket failed: " << errno << std::endl;
        return -1;
    }
    std::cout << "[debug]: create server socket complete." << std::endl;

    ret = bind(listener, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
    if (ret < 0) {
        std::cerr << "[error]: bind address failed: " << ret << std::endl;
        return -1;
    }
    std::cout << "[debug]: create bind address complete." << std::endl;

    ret = listen(listener, CONNECTION_PENDING_LIMIT);
    if (ret < 0) {
        std::cerr << "[error]: listen socket failed: " << ret << std::endl;
        return -1; 
    }
    std::cout << "[debug]: start listen: " << serverIP << ":" << port << std::endl;

    epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        std::cerr << "[error]: epoll instance create failed: " << errno << std::endl;
        return -1;
    }
    std::cout << "[debug]: epoll_create complete." << std::endl;

    addfd(socket);

    while (true) {
        epollEventsCount = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (epollEventsCount < 0) {
            std::cerr << "[error]: epoll wait error: " << errno << std::endl;
            ret = -errno;
            break;
        }
        std::cout << "[debug]: get " << epollEventsCount << " events..." << std::endl;

        for (int i = 0; i < epollEventsCount; i++) {
            int sfd = events[i].data.fd;
            std::cout << "[debug]: socket fd: " << sfd;

            if (sfd == listener) {
                clientAddrLen = sizeof(struct sockaddr_int);
                std::cout << "[debug]: client addr length: " << clientAddrLen;

                clientFd = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
                addfd(clientFd);
            } else {
                // TODO: 构造task
                int cfd = sfd;
                len = recv(cfd, buffer, BUFFER_SIZE, 0);
                if (len == 0) {
                    // Close fd
                } else {
                    struct Task *task = (struct Task*)malloc(sizeof(struct Task));
                    BufferToTask(buffer, task);
                    tPool.enqueue(task);
                }
            }
        }
    }

    close(epfd);
    close(listener);
}

#endif