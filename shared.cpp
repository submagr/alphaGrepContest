#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include "myqueue.h"
using namespace std;

mutex mu1;
mutex mu2;

myQueue qr1;
myQueue qr2;

void insertQ(int num ){
    if(num==1){
        for(int i=1; i<=5; i++){
            mu1.lock();
            qr1.push(i);
            mu1.unlock();
        }
        mu1.lock();
        qr1.push(0);
        mu1.unlock();
    }else{
        for(int i=100; i<=106; i++){
            mu2.lock();
            qr2.push(i);
            mu2.unlock();
        }
        mu2.lock();
        qr2.push(0);
        mu2.unlock();
    }
}

void readQ(){
    int currentRead = -1;
    while(currentRead != 0){
        mu1.lock();
            if(!qr1.isEmpty()){
                currentRead = qr1.front();
                qr1.pop(); 
                cout << "From qr1: " << currentRead << endl;
            }
        mu1.unlock();
    }
    currentRead = -1;
    while(currentRead != 0){
        mu2.lock();
            if(!qr2.isEmpty()){
                currentRead = qr2.front();
                qr2.pop(); 
                cout << "From qr2: " << currentRead << endl;
            }
        mu2.unlock();
    }
}

int main(){
	thread r1(insertQ, 1);
	thread r2(insertQ, 2);
    thread w1(readQ);
    r1.join();
    r2.join();
    w1.join();
}
