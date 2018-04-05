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
    Node* prev = j;
    while(i != NULL) {
        while(j != NULL) {
            if(i->getID() == j->getID()) {      //if their letter is equal to something already in my table
                if(i->getCost() + nextHopCost < j->getCost()) { //if their cost is less than my cost
                    j->setCost(i->getCost() + nextHopCost); //update cost to their cost
                    j->setPort(clientPort);     //set port to route through them
                }
                break; //get outta this while
            }
            prev = j;
            j = j->getNext();
        } //if reached here, and j is NULL, never found equal. so we add this entry
        if(j == NULL & i->getID() != nodeLetter) {
            j = new Node; //create new node
            j->setDest(i->getID()); //set its dest as i of theirNodes
            j->setPort(clientPort);             //route through them
            j->setCost(i->getCost() + nextHopCost);
            
            prev->setNext(j); //ADD IT TO THE TABLE
            
            //cout << "DEST: " << j->getID() << " PORT: " << j->getPort() << " COST: " << j->getCost() << endl;
        }
        i = i->getNext();                       //go to next of their nodes
        j = myNodes->getNext();                 //back to beginning of my nodes
    }
    outputMessage( &r, buf, nodeLetter );

}


//or maybe return char array?
string BellmanFord::makeDV(RoutingTable r, char nodeLetter) {
    string DV = "~d ";
    string cost;
    Node* myNodes = r.getMyNodes(nodeLetter);   //get its own routing table
    Node* temp = myNodes->getNext();
    //format: each line is dest letter, cost (thassit)
    while(temp != NULL) {
        DV += temp->getID();                    //add dest character
        DV += ",";
        cost = to_string(temp->getCost());
        DV += cost;                //add cost (see what the heck this does), may need int to char conversion
        DV += "\n";                             //newline to separate
        temp = temp->getNext();                 //go to next in list
    }
    //char* dv_c = (char*)DV.c_str();
    //cout << "dv: " << dv_c << endl;
    return DV;
}

Node* BellmanFord::parseDV(char* buf) {
    //cout << "ENTERED FUNCTION" << endl;
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
    
    //cout << "letter first: " << buf[i] << endl;
    
    while (buf[i] != ',' ) {
        i++;
    }                                           //now it's a comma
    i++;                                        //now it's past the comma
    
    
    cost = buf[i] - '0';                        //convert char to int
    head->setCost(cost);
    
    //cout << "cost first: " << buf[i] << endl;
    
    while (buf[i] != '\n' ) {
        i++;
        //cout << "IN WHILE TO GET NEWLINE\n";
    }                                       //now it's a newline
    i++;                                    //now it's past newline
    
    Node* prev = head;
    prev->setNext(temp);
    
    //now get the rest
    while(buf[i] != '\0') {                     //while not end of string
        temp = new Node;
        temp->setDest(buf[i]);                 //set dest letter
        
        //cout << "DEST LETTER" << buf[i] << endl;
        
        while (buf[i] != ',' ) {
            i++;
        }                                       //now it's a comma
        i++;                                    //now it's past the comma
        
        
        cost = buf[i] - '0';                    //convert char to int
        temp->setCost(cost);
        
        //cout << "COST" << buf[i] << endl;
        
        prev->setNext(temp);                    //link it to list
        
        prev = temp;                            //get ready to add next node
        while (buf[i] != '\n' ) {
            i++;
            //cout << "IN WHILE TO GET NEWLINE\n";
        }                                       //now it's a newline
        i++;                                    //now it's past newline
    }
    return head;
}


void outputMessage(RoutingTable* routingtable, char vBuff[], char myLetter ){
    ofstream data;
    
    stringstream fileName;
    fileName.str("");
    fileName << "router-output-" << myLetter << ".txt";
    
    
    data.open( fileName.str() );
    
    if(!data.is_open())
        cout << "Not open";
    
    if(vBuff[0]=='~' && vBuff[1]=='d'){    
    Node* temp;
    //for(int i = 0; i < routingtable->getSize(); i++){
    temp = (routingtable->getArray(myLetter - 65))->getHead();
        while(temp -> getNext() != NULL){
            data << temp -> getID();
            data << ",";
            data << temp -> getNext() -> getID();
            data << ",";
            data << temp -> getNext() -> getPort();
            data << "," << temp -> getNext() -> getCost() << endl;
            
            temp = temp -> getNext();
        }        
    
    }
    
    else{
        for(int i = 0; i< sizeof(vBuff) / sizeof(vBuff[0]) ; i++){
            data << vBuff[i];
        }
    }
    
}