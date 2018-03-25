#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>         
#include <sys/socket.h>
using namespace std;

class Node{             // These represent the nodes that a particular node is connected to
    private:
        Node* next;     // They'll be in a linked list format so we have next and previous links
        Node* prev;
        char dest;      // Each node has a letter representing it
        int port;       // Each node has a port number
        int cost;       // There's an associated cost going to this node

    public:
        Node();
        // Accessor and mutator
        Node* getNext(){ return next; }
        Node* getPrev(){ return prev; }
        int getPort(){ return port; }
        int getCost(){ return cost; }
        char getID(){ return dest; }

        void setNext(Node* a){ next = a; }
        void setPrev(Node* a){ prev = a; }
        void setDest(char a){ dest = a; }
        void setPort(int a){ port = a; }
        void setCost(int a){ cost = a; }
};

Node* newNode(char nodeletter){
    Node* a = new Node;
    a -> setDest(nodeletter); 
    return a;
}

Node::Node(){
    next = NULL;
    prev = NULL;
    port = 0;   
    cost = 0;    
}

class NodeList{
    private:
        Node* head;

    public:
        NodeList(){ head = NULL; }
        //accessor mutator
        Node* getHead(){ return head; }
        void setHead(Node* a){ head = a; }
};

class RoutingTable{
    private:
        int size;
        NodeList* array;

    public:
        RoutingTable();  // Make the defualt constructor read in the initial values
        void makeRoutingTable();
        void parseData();
        void addEdge(int w, int port, char star, char destination);
        void printTable();
        NodeList* getArray(int a){ return array + a; }
};

RoutingTable::RoutingTable(){
    size = 6;
    array = NULL;   // Here the routing tables size can be changed   
}

void RoutingTable::makeRoutingTable(){
    array = (NodeList*)malloc(6 * sizeof(NodeList));
    for(int i = 0; i < size; i++){
        array[i].setHead(newNode(char(i + 65)));    // Here I am making every head node in the routing table.
        //array[i].setHead(newNode(char(i + 65)));
    } 
}

void RoutingTable::addEdge(int w, int port, char start, char destination){
    
    if(array[start-65].getHead() -> getNext() == NULL){ // If there is no head node then new node is made the head node
        array[start-65].setHead(newNode(start));
        array[start-65].getHead() -> setNext(newNode(destination));
        array[start-65].getHead() -> getNext() -> setCost(w);
        array[start-65].getHead() -> getNext() -> setPort(port);
        array[start-65].getHead() -> setPrev(NULL); 
        array[start-65].getHead() -> getNext() -> setPrev(array[start-65].getHead());
        return;
    }
    
    Node* temp = array[start-65].getHead();
    while(temp -> getNext() != NULL){       // Loops through the linked list untin the end is reached
        temp = temp -> getNext();           // The node is appended to the end of the linked list
    }
    
    Node* end = newNode(destination);
    temp -> setNext(end);
    end -> setPrev(temp);
    end -> setCost(w);
    end -> setPort(port);
}

void RoutingTable::parseData(){
    FILE* data;
    data = fopen("routingtable.txt", "r");
    
    if(data == NULL)
        cout << "Data is NULL \n";
        
    int n = 2;
    string portnum;
    string buffer;
    string weight;
    char start, destination;
    int w = 0, p = 0;
    char a = (char) fgetc(data);
    
    while (!feof(data)){            // If the EOF is reached, stop reading
        if(a == ',' || a == '\n'){
            a = (char)fgetc(data);  // If the character is  useless, get the next char
        }
        if(a >= 0x41){              // This character is a letter and should be added to the start and destination variables.
            start = a;
            a = fgetc(data);
            while(a < 0x41)
                a = (char)fgetc(data);
            destination = a;
        }
        else{
            if(portnum.length() == 5){      // The portnumber is only 5 characters long
                weight = a;
                stringstream s (weight);    // Here I am converting string to int
                stringstream q (portnum); 
                s >> w;
                q >> p;
                addEdge(w, p, start, destination);
                portnum.erase(portnum.begin(), portnum.end());
            }
            else{
                portnum = portnum + a;
            }
        }
        buffer = buffer + a;
        a = fgetc(data);
    }
    fclose(data);
}

void RoutingTable::printTable(){
    Node* head;
    for(int i = 0; i < size ; i++){
        head = array[i].getHead();
        while(head != NULL){
            cout << head -> getID() << ", " << head -> getPort() << ", " << head -> getCost() << endl;
            head = head -> getNext();
        }
        cout << endl;
    }
}

/* int main(int argc, char* argv[]){
    RoutingTable routingtable;
    routingtable.makeRoutingTable();
    routingtable.parseData();
    routingtable.printTable();
    return 0;   
} */
