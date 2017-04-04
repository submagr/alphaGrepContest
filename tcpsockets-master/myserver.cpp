#include <iostream>
#include <stdlib.h>
#include "tcpacceptor.h"
#include "myqueue.h"
#include <strings.h>
#include <thread>
#include <mutex>

const int n = 2; // number of writer threads
const int fileSize = 3;
int f[n][fileSize];

mutex g_lockprint;

long getRealData(char* buff, int *exchangeNum){
    // skip 192 bits = 24 bytes and read 12 bytes from there
    *exchangeNum = *(int *)(buff+23);
    return *(long *)(buff+27);
}

void handleConnection(TCPStream* stream){
    {
        unique_lock<mutex> locker(g_lockprint); 
        cout << "thread " << "started" << endl;
    }
    ssize_t len;
    char line[256]; 
    bzero(line, 256);
    int exchangeNum;
    long exchangeInt;
    while ((len = stream->receive(line, sizeof(line))) > 0) {
        line[len] = '\0';
        exchangeInt = getRealData(line, &exchangeNum);
        unique_lock<mutex> locker(g_lockprint); 
        cout << "Thread received ------ " << endl << line << "^-------" << endl << "Received data is: Exchange num: " << exchangeNum << ", Integer: " << exchangeInt << endl;
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
