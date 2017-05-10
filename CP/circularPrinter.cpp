/* Author: Shubham Agrawal 
 * Email: agshubh191@gmail.com
 * Details: There is global variable turn (safeguarded by mutex g_lockturn). Every thread waits for turn. On getting turn, It prints it's respactive part, updates turn.
 *
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <string.h>
using namespace std;

mutex g_lockturn;
char *str;
int numCharsToPrint;
int n;
int lenStr;
int printTarget;
int turn;
int currIndex;

void myPrint(int id){
    cout << "Thread" << id << ": ";
    for(int i=currIndex; i< currIndex+numCharsToPrint; i++){
        cout << str[i%lenStr] ;
    }
    cout << endl << endl;
}

void handler(int id, int *printed){
    while(printed[id] != printTarget){
        // wait for it's turn
        do{
            unique_lock<mutex> locker(g_lockturn); 
        }while(turn != id);
        myPrint(id);
        currIndex = (currIndex + numCharsToPrint ) %lenStr;
        printed[id] +=1;
        turn = (id+1)%n;
    }
}

int main(int argc, char *argv[]){
    if(argc != 5){
        cout << "Need 4 arguments." << endl;
        cout << "Usage: ./circularPrinter <string> <#characters to print at a time> <#threads> <#times each thread will print>";
        exit(1);
    }

    str = argv[1] ;
    lenStr = strlen(str);
    numCharsToPrint  = atoi(argv[2]);
    n                = atoi(argv[3]);
    printTarget = atoi(argv[4]);

    int printed[n];
    currIndex = 0;
    turn = 0;
    thread *threads[n];
    for(int i=0; i<n; i++){
        printed[i] = 0;
        threads[i] = new thread(handler, i, (int *)printed);
    }
    for(int i=0; i<n; i++){
        threads[i]->join();
        delete threads[i];
    }
    return 0;
}
