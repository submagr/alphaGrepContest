#include <stdio.h>
#include <stdlib.h>
#include "tcpacceptor.h"
#include "myqueue.h"
#include <thread>

const int n = 2; // number of writer threads
const int fileSize = 3;
int f[n][fileSize];

myQueue q[n];
// mutex g_lockprint;
// mutex g_lockqueue[n];
// condition_variable g_queuecheck[n];
// bool finished[n];
// int currentTop[n];
// #define MAXINT  1000000; // TODO

long getRealData(char* buff, int *exchangeNum){
    *exchangeNum = *(int *)(buff+24);
    return *(long *)(buff+28);
}

void handleConnection(int id, TCPStream* stream){
    printf("thread started\n");
    ssize_t len;
    char line[256]; // skip 192 bits = 24 bytes and read 12 bytes from there
    while ((len = stream->receive(line, sizeof(line))) > 0) {
        line[len] = 0;
        printf("received - %s\n", line);
        int exchangeNum, exchangeInt;
        exchangeInt = getRealData(line, &exchangeNum);
        // stream->send(line, len);
        cout << "data received: Exchange num: " << exchangeNum << ", Integer: " << exchangeInt << endl;
        // q[id].push(exchangeInt);
    }
    delete stream;
}

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 4) {
        printf("usage: server <port> [<ip>]\n");
        exit(1);
    }

    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (argc == 3) {
        acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else {
        acceptor = new TCPAcceptor(atoi(argv[1]));
    }
    if (acceptor->start() == 0) {
        myQueue<thread *> q;
        while (1) {
            stream = acceptor->accept();
            if (stream != NULL) {
                thread *t = new thread(handleConnection, stream);
                q.push(t);
            }
        }
        while(!q.isEmpty()){
            thread *t = q.front();
            t->join();
            delete t; 
            q.pop();
        }
    }
    exit(0);
}
