#include <iostream>
#include "ThreadPool.h"

struct Request {
    int a;
    int b;
    int r;
};

void SumRequest(void *arg)
{
    struct Request *req = (struct Request *)arg;
    req->r = req->a + req->b;
    std::cout << req->a << " + " << req->b << " = " << req->r << std::endl;
    return ; 
}

int main(int argc, char *argv[])
{
    ThreadPool tPool(4);

    for (int i = 0; i < 8; i++) {
        struct Request *req = (struct Request*)malloc(sizeof(struct Request));
        req->a = i + 1;
        req->b = i + 2;
        req->r = 0;

        Task *task = (struct Task*)malloc(sizeof(struct Task));
        task->func = SumRequest;
        task->args = (void *)req;

        tPool.EnQueue(task);
        sleep(1);
    }

    while(1) {
        
    }

    return 0;
}