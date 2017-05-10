#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <strings.h>
#include "tcpconnector.h"
#include "myqueue.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <strings.h>
using namespace std;

const int n = 2; // number of writer threads
const int fileSize = 3;
long data[n][fileSize];

mutex g_lockprint;

char * genPacket(char *buff, int exchangeNum, long exchangeInt){
    // skip 192 bits = 24 bytes and read 12 bytes from there 
    *(int *)(buff+24) = exchangeNum;
    *(long *)(buff+28) = exchangeInt;
}

void handleClient(int id, TCPStream *stream){
    char tcpPacket[256]; 
    bzero(tcpPacket, 256);
    for(int j=0; j<fileSize; j++){
        genPacket(tcpPacket, id, data[id][j]);
        stream->send(tcpPacket, 256);

        // Print to stdout
        unique_lock<mutex> locker(g_lockprint); 
        cout << tcpPacket << endl;
    }
    delete stream;
}

void simulateFileData(){
    data[0][0] = 1;
    data[0][1] = 3;
    data[0][2] = 0;

    data[1][0] = 2;
    data[1][1] = 4;
    data[1][2] = 0;
}

int main(int argc, char** argv)
{
    simulateFileData;
    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    myQueue<thread *> q;
    for(int i=0; i<n; i++){ // Create n client connections
        TCPConnector* connector = new TCPConnector();
        TCPStream* stream = connector->connect(argv[2], atoi(argv[1]));
        if (stream) {
            thread *t = new thread(handleClient, i, stream);
            q.push(t);
        }
    }
    while(!q.isEmpty()){
        thread *t = q.front();
        t->join();
        delete t; 
        q.pop();
    }
    exit(0);
}
