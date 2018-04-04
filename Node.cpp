#include "Node.h"

Node::Node(){
    next = NULL;
    prev = NULL;
    port = 0;   
    cost = 0;    
}

Node* newNode(char nodeletter){
    Node* a = new Node;
    int port = 10000 + (int)(nodeletter - 65);
    a -> setDest(nodeletter); 
    a -> setPort(port);
    return a;
}