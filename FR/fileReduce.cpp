#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
using namespace std;

class myRow;
class packet;
myRow** hashedArray;
vector<packet *> packets;
vector<myRow *> rawFile;
int maxId;

class myRow{
    public: 
        bool isRoot;
        myRow();
    
        // Parsed INFO: type,symbol,price,quantity,expirydate,strikeprice,amendtime,id, parentid
        string type;
        string symbol; 
        string price;
        string quantity;
        string expirydate;
        string strikeprice;
        string amendtime;
        int id;
        int parentid;

        myRow *parent;
        vector<myRow *> children; // List of children pointers

        string getString();   // Return entire string from which this row comes
        int getSubtreeSize(); // Return size of subtree including root
        string getSubtreeAsString(); // Return subtree as concatenation of individual strings
};

myRow::myRow():isRoot(true), parent(NULL){}

string myRow::getString(){
    return type + ',' + symbol + ',' + price + ',' + quantity + ',' + 
                 expirydate + ',' + strikeprice + ',' + amendtime + ',' + to_string(id) 
                 + ',' + to_string(parentid);
}

int myRow::getSubtreeSize(){
    int size = 1; // Include self
    for(int i=0; i<children.size(); i++){
        size += children[i]->getSubtreeSize();
    }
    return size;
}

string myRow::getSubtreeAsString(){
    string res = getString(); 
    for(int i=0; i<children.size(); i++){
        res = res + "\n" + children[i]->getSubtreeAsString();
    }
    return res;
}

class packet{ // This is required to avoid calculation of size repeatedly
    public: 
        int size;
        myRow *root; 
};

void getPackets(){
    for(int i=0; i<=maxId; i++){
        myRow *current = hashedArray[i];
        if(current != NULL and current->isRoot){
            packet *p = new packet();
            p->size = current->getSubtreeSize();
            p->root = current;
            packets.push_back(p);
        }
    }
}

void getHashedFile(){
    hashedArray = new myRow * [maxId+1];
    for(int i=0; i<=maxId; i++)
        hashedArray[i] = NULL;
    for(int i=0; i<rawFile.size(); i++){
        hashedArray[rawFile[i]->id] = rawFile[i];
    }
    for(int i=0; i<=maxId; i++){
        myRow *current = hashedArray[i];
        if(current != NULL){
            if(current->type.compare("P") == 0){ // Node is child
                current->isRoot = false;
                current->parent = hashedArray[current->parentid];
                hashedArray[current->parentid]->children.push_back(current);
            }
        }
    }
}

void readInputFile(char *fileName){
    ifstream file(fileName);
    string str; 

    maxId = INT_MIN;
    while (getline(file, str))
    {
        if(str.size() == 0)
            continue;

        myRow *row = new myRow;

        istringstream iss(str);

        getline(iss, row->type, ',');
        getline(iss, row->symbol, ',');
        getline(iss, row->price, ',');
        getline(iss, row->quantity, ',');
        getline(iss, row->expirydate, ',');
        getline(iss, row->strikeprice, ',');
        getline(iss, row->amendtime, ',');
        string s;
        getline(iss, s, ',');
        row->id = stoi(s);
        getline(iss, s, ',');
        row->parentid = stoi(s);

        rawFile.push_back(row);

        if(maxId < row->id)
            maxId = row->id;
    }
}

int main(int argc, char **argv){
    if(argc < 3){
        cout << "Usage: ./fileReduce <inputFileName> <X: Capacity of smaller file>";
        exit(1);
    }
    // Read entire file
    // vector<myRow *> rawFile = readInputFile(argv[1]);
    // for(int i=0; i<rawFile.size(); i++){
    //     cout << rawFile[i]->getString() << endl;
    // }
    
    readInputFile(argv[1]); 
    // Print read lines
    // for(int i=0; i<rawFile.size(); i++){
    //     cout << rawFile[i]->getString() << endl;
    // }
    getHashedFile();
    getPackets();
    // Print grouped packets
    // for(int i=0; i<packets.size(); i++){
    //     cout << "Packet " << i << " contains: " << endl;
    //     cout << packets[i]->root->getSubtreeAsString() << endl << endl;
    // }
    // 
    // Read each line in class row
    // Club rows into packets
    // Sort packets according to their size
    // Implement algorithm
    // Write boxes back in file
    // 12 T,SBIBANK,1000,100,20121210,120,20121209103032,1236,0
    // 14 P,ICICIBANK,1100,100,20121210,120,20121209103030,1237,1234     
    // TODO: Delete hashedarray, packets
    return 0; 
}
