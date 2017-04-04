#include <iostream>
#include <stdlib.h>
#include "tcpacceptor.h"
#include "myqueue.h"
#include <strings.h>
#include <thread>
#include <mutex>
#include <condition_variable>

const int n = 4; // number of writer threads

myQueue<long> q[n];
mutex g_lockprint;
mutex g_lockqueue[n];
condition_variable g_queuecheck[n];
bool finished[n];
int currentTop[n];
#define MAXINT  1000000; // TODO

int printMin(){
    int minValue = MAXINT;
    int minIndex = 0;
    for(int i=0; i<n; i++){
        if(!finished[i]){
            if(currentTop[i] < minValue){
                minValue = currentTop[i];
                minIndex = i;
            }
        }
    }

    {   // print min value
        unique_lock<mutex> locker(g_lockprint); 
        cout << currentTop[minIndex] << endl;
    }

    {   // Pop from minIndex queue
        unique_lock<mutex> locker(g_lockqueue[minIndex]);
        q[minIndex].pop();
    }

    return minIndex;
}

bool allFinished(){
    for(int i=0; i<n; i++){
        if(!finished[i])
            return false;
    }
    return true;
}

void mergerFunc()
{
    for(int i=0; i<n; i++)
        finished[i] = false;

    for(int i=0; i<n; i++){
        unique_lock<mutex> locker(g_lockqueue[i]); 
        while(q[i].isEmpty()){
            g_queuecheck[i].wait(locker);
        }
        currentTop[i] = q[i].front();

        // {   
        //     unique_lock<mutex> locker(g_lockprint); 
        //     cout << "Reader function start: " << i << " : " << currentTop[i] << endl;
        // }

    }

    // update finished
    for(int i=0; i<n; i++){
        if(currentTop[i] == 0){
            finished[i] = true;
        }
    }

    while(!allFinished()){
        int minIndex = printMin();
        {
            unique_lock<mutex> locker(g_lockqueue[minIndex]); 
            while(q[minIndex].isEmpty()){
                g_queuecheck[minIndex].wait(locker);
            }
            currentTop[minIndex] = q[minIndex].front();
        }
        if(currentTop[minIndex] == 0)
            finished[minIndex] = true;
    }
}

long getRealData(char* buff, int *id){
    // skip 192 bits = 24 bytes and read 12 bytes from there
    *id = *(int *)(buff+23);
    return *(long *)(buff+27);
}

void handleConnection(TCPStream* stream){
    {   // Print thread start summary
        unique_lock<mutex> locker(g_lockprint); 
        cout << "thread " << "started" << endl;
    }
    ssize_t len;
    char line[256]; 
    bzero(line, 256);
    int id;
    long data;
    while ((len = stream->receive(line, sizeof(line))) > 0) {
        line[len] = '\0';
        data = getRealData(line, &id);

        {   // Push received data on respactive queue and notify any threads waiting on it
            unique_lock<mutex> locker(g_lockqueue[id]);
            q[id].push(data);
            g_queuecheck[id].notify_one();
        }

        {   // Print received data summary
            unique_lock<mutex> locker(g_lockprint); 
            cout << "Thread received ------ " << endl << line << "^-------" << endl << "Received data is: Exchange id: " << id << ", Integer: " << data << endl;
        }

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
        // Start merger thread
        thread mergerthread(mergerFunc);

        for(int i=0; i<n; i++){ // Accept n connections
            stream = acceptor->accept();
            if (stream != NULL) {
                thread *t = new thread(handleConnection, stream);
                q.push(t);
            }
        }
        
        // Close threads gracefully
        mergerthread.join();

        while(!q.isEmpty()){  
            thread *t = q.front();
            t->join();
            delete t; 
            q.pop();
        }
    }
    exit(0);
}
