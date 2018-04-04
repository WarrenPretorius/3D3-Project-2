#include "bellmanford.h"

void BellmanFord::bellmanFord(char* buf, RoutingTable r, int clientPort, char nodeLetter) {
    //first parse message in buf, using helper function
    //---->INTO a new nodelist (routing table).
    //then we compare all nodelist entries with current node's routing table
    Node* theirNodes = parseDV(buf);
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
                if(i->getCost() + nextHopCost < j->getCost()) { //if their cost is less than my cost
                    j->setCost(i->getCost() /*+ cost to them*/); //update cost to their cost
                    j->setPort(clientPort);     //set port to route through them
                    cout << "FOUND EQUAL" << endl;
                    break;                      //get outta this while
                }
            }
            j = j->getNext();
        } //if reached here, and j is NULL, never found equal. so we add this entry
        if(j == NULL) {
            cout << "CREATING NEW NODE";
            j = new Node; //create new node
            j->setDest(i->getID()); //set its dest as i of theirNodes
            j->setPort(clientPort);             //route through them
            j->setCost(i->getCost() + nextHopCost);
        }
        i = i->getNext();                       //go to next of their nodes
    }
}


//or maybe return char array?
string BellmanFord::makeDV(RoutingTable r, char nodeLetter) {
    string DV = "~d ";
    Node* myNodes = r.getMyNodes(nodeLetter);   //get its own routing table
    Node* temp = myNodes->getNext();
    string cost;
    //format: each line is dest letter, cost (thassit)
    while(temp != NULL) {
        DV += temp->getID();                    //add dest character
        DV += ",";
        cost = to_string(temp->getCost());
        //cout << "COST, IN makeDV: " << cost << endl;
        DV += cost;                  //add port (see what the heck this does), may need int to char conversion
        DV += "\n";                             //newline to separate
        temp = temp->getNext();                 //go to next in list
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
    Node* head = new Node;
    head->setDest(buf[i]);
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
        temp = new Node;
        temp->setDest(buf[i]);                 //set dest letter
        
        while (buf[i] != ',' ) {
            i++;
        }                                       //now it's a comma
        i++;                                    //now it's past the comma
        
        cost = buf[i] - '0';                    //convert char to int
        temp->setCost(cost);
        prev->setNext(temp);                    //link it to list
        prev = temp;                            //get ready to add next node
    }
    return head;
}

//int main () {
//    RoutingTable routingtable;
//    routingtable.makeRoutingTable();
//    routingtable.parseData();
//    routingtable.printTable();
//    
//    
//    return 0;
//}







