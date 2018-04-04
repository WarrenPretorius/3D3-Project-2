//
//  bellmanford.cpp
//  
//
//  Created by Sophia Rosney on 22/03/2018.
//
//


//what is structure of "graph"??
//each router is a "node" - it is "src"
//has letter (name) and portnum and cost to each name, portnum
//list of neighbours: name, port, cost
//how do we know its neighbours?? look in routing table. must we define neighbours?

#include <stdio.h>
#include "bellmanford.h"
using namespace std;
//check if DV is to itself???? if B computes lower cost for A, A changes its cost to B
//send entries like: dest,cost. that's all we need

//WHAT THE HELL DOES A DV MESSAGE LOOK LIKE. it is whole routing table
//dv is ~d D10000, 3
//rename to RECEIVE DV

// BUFF CONTAINS DV. only do this when get new DV. mad
//think we need a static ID in my_router for each node

//maybe int BellmanFord?? return 1 if table was changed, 0 if nothing was
void BellmanFord::bellmanFord(char* buf, RoutingTable r, int clientPort, char nodeLetter) {
    //first parse message in buf, using helper function
    //---->INTO a new nodelist (routing table).
    //then we compare all nodelist entries with current node's routing table
    Node* theirNodes /* = parseDV*/;
    //get their letter
    
    char theirLetter = clientPort - 10000 +65 ; //get from clientPort
    int nextHopCost;                            //cost to them
    
    Node* myNodes = r.getMyNodes(nodeLetter);   //get its own routing table
    //first entry of myNodes is itself with a cost of 0. so go to next
    Node *j = myNodes->getNext();               //for each entry in my table, excluding myself

    while(j != NULL) {
        if(theirLetter == j->getID()) {
            nextHopCost = j->getCost();
        }
        j = j->getNext();
    }
    
    //check if the slot is NULL
        
    Node* i = theirNodes;                       //for each entry in their table
    //first entry of myNodes is itself with a cost of 0. so go to next
    j = myNodes->getNext();                     //for each entry in my table, excluding myself
    while(i != NULL) {
        while(j != NULL) {
            if(i->getID() == j->getID()) {      //if their letter is equal to something already in my table
                if(i->getCost() /*+ cost to them*/ < j->getCost()) { //if their cost is less than my cost
                    j->setCost(i->getCost() /*+ cost to them*/); //update cost to their cost
                    j->setPort(clientPort);     //set port to route through them
                    cout << "FOUND EQUAL" << endl;
                    break;                      //get outta this while
                }
            }
            j = j->getNext();
        } //if reached here, and j is NULL, never found equal. so we add this entry
        if(j == NULL) {
            j = newNode(i->getID());
            j->setPort(clientPort);             //route through them
            j->setCost(i->getCost() /*+ cost to them*/);
        }
        i = i->getNext();                       //go to next of their nodes
    }
}


//or maybe return char array?
string BellmanFord::makeDV(RoutingTable r, char nodeLetter) {
    string DV = "~d ";
    Node* myNodes = r.getMyNodes(nodeLetter);   //get its own routing table
    Node* temp = myNodes->getNext();
    //format: each line is dest letter, cost (thassit)
    while(temp != NULL) {
        DV += temp->getID();                    //add dest character
        DV += ",";
        DV += temp->getCost();                  //add port (see what the heck this does), may need int to char conversion
        DV += "\n";                             //newline to separate
    }
    
    return DV;
}

Node* BellmanFord::parseDV(char* buf) {
    NodeList table;
    int cost;
    //go past initial ~d
    int i = 0;
    while(i < 3){                               //go past buf[0], [1], [2]
        i++;
    }

    //A,3\nB,4\n....etc
    //all the ports are unimportant here. only use dest letter and cost
    Node* head = newNode(buf[i]);
    Node* temp = NULL;
    
    while (buf[i] != ',' ) {
        i++;
    }                                           //now it's a comma
    i++;                                        //now it's past the comma
    
    cost = buf[i] - '0';                        //convert char to int
    head->setCost(cost);
    
    Node* prev = head;
    prev->setNext(temp);
    
    //now get the rest
    while(buf[i] != '\0') {                     //while not end of string
        while (buf[i] != '\n' ) {
        i++;
        }                                       //now it's a newline
        i++;                                    //now it's past newline
        temp = newNode(buf[i]);                 //set dest letter
              
        while (buf[i] != ',' ) {
        i++;
        }                                       //now it's a comma
        i++;                                    //now it's past the comma
        
        cost = buf[i] - '0';                    //convert char to int
        temp->setCost(cost);
        prev->setNext(temp);                    //link it to list
        prev = temp;                            //get ready to add next node
    }
    
}








