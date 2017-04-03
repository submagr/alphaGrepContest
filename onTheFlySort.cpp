#include<stdio.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <random>
#include "myqueue.h"
#include <climits>
using namespace std;
 
const int n = 2; // number of writer threads
const int fileSize = 3;
int f[n][fileSize];

myQueue q[n];
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

    return minIndex;
}


bool allFinished(){
    for(int i=0; i<n; i++){
        if(!finished[i])
            return false;
    }
    return true;
}

void readerFunc()
{
    for(int i=0; i<n; i++)
        finished[i] = false;

    for(int i=0; i<n; i++){
            unique_lock<mutex> locker(g_lockqueue[i]); 
            while(!q[i].isEmpty()){
                g_queuecheck[i].wait(locker);
            }
            currentTop[i] = q[i].front();
            q[i].pop();
    }

    // update finished
    for(int i=0; i<n; i++){
        if(currentTop[i] == 0){
            finished[i] = true;
        }
    }

    int minIndex = printMin();

    while(!allFinished()){
        {
            unique_lock<mutex> locker(g_lockqueue[minIndex]); 
            while(!q[minIndex].isEmpty()){
                g_queuecheck[minIndex].wait(locker);
            }
            currentTop[minIndex] = q[minIndex].front();
            q[minIndex].pop();
        }
        if(currentTop[minIndex] == 0)
            finished[minIndex] = true;
        minIndex = printMin();
    }
}

void writerFunc(int id, mt19937& generator){
    // print a starting message
    {
        unique_lock<mutex> locker(g_lockprint);
        cout << "[worker " << id << "] \t running..." << endl;
    }
 
    for(int i=0; i<fileSize; i++){
        // simulate packet receive time 
        this_thread::sleep_for(chrono::seconds(1 + generator() % 5));

        unique_lock<mutex> locker(g_lockqueue[id]);
        q[id].push(f[id][i]);
        g_queuecheck[id].notify_one();
    }
}


int main(){
    f[0][0] = 1;
    f[0][1] = 3;
    f[0][2] = 0;

    f[1][0] = 2;
    f[1][1] = 4;
    f[1][2] = 0;
    // initialize a random generator
    mt19937 generator((unsigned int)chrono::system_clock::now().time_since_epoch().count());
 
    // start the reader 
    thread readerthread(readerFunc);
 
    // start the writer threads
    thread threads[n];
    for(int i=0; i<n; i++)
    {
        threads[i] = thread(writerFunc, i, ref(generator)) ;
    }
 
    // wait for all the writer threads to finish 
    for(int i=0; i<n; i++)
    {
        threads[i].join();
    }
    
    // wait for reader thread to finish
    readerthread.join();
 
    return 0;
}
