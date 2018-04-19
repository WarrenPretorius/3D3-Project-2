
#include "bellmanford.h"

// Maybe int BellmanFord?? return 1 if table was changed, 0 if nothing was
void BellmanFord::bellmanFord(char* buf, RoutingTable r, int clientPort, char nodeLetter) {
    // First parse message in buf, using helper function
    //---->INTO a new nodelist (routing table).
    // Then we compare all nodelist entries with current node's routing table

    Node* theirNodes = parseDV(buf);

    // Get their letter
    char theirLetter = clientPort - 10000 +65 ; // Get from clientPort
    int nextHopCost;                            // Cost to them
    Node* myNodes = r.getMyNodes(nodeLetter);   // Get its own routing table
    
    //first entry of myNodes is itself with a cost of 0. so go to next
    Node *j = myNodes->getNext();               // For each entry in my table, excluding myself
    
    while(j != NULL) {
        if(theirLetter == j->getID()) {     // Nexthopcost from my table
            nextHopCost = j->getCost();     // My cost to them
        }
        j = j->getNext();
    }
    
    // Check if the slot is NULL
    
    Node* i = theirNodes;       // For each entry in their table
                                // First entry of myNodes is itself with a cost of 0. so go to next
    j = myNodes->getNext();     // For each entry in my table, excluding myself
    Node* prev = j;
    while(i != NULL) {
        while(j != NULL) {
            if(i->getID() == j->getID()) {                      // If their letter is equal to something already in my table
                if(i->getCost() + nextHopCost < j->getCost()) { // If their cost is less than my cost
                    j->setCost(i->getCost() + nextHopCost);     // Update cost to their cost
                    j->setPort(clientPort);                     // Set port to route through them
                }
                break;                                      // Get outta this while
            } else if(i->getID() == nodeLetter) {           // If it is their connection to myself
                if(i->getCost() < nextHopCost) {            // If their cost to me is less than my cost to them
                                                            // (if cost between us has changed)
                    nextHopCost = i->getCost();             // Update nextHopCost
                    while(j != NULL) {                      // Then update cost to them in my table
                        if(theirLetter == j->getID()) {
                            nextHopCost = j->getCost();
                        }
                        j = j->getNext();
                    }
                    i = theirNodes;                 // Go back to beginning of table comparison
                    j = myNodes->getNext();
                    break;
                }
            }
            prev = j;
            j = j->getNext();
        } // If reached here, and j is NULL, never found equal. so we add this entry
        
        if(j == NULL & i->getID() != nodeLetter) {
            j = new Node;                           // Create new node
            j->setDest(i->getID());                 // Set its dest as i of theirNodes
            j->setPort(clientPort);                 // Route through them
            j->setCost(i->getCost() + nextHopCost);
            
            prev->setNext(j);                       //ADD IT TO THE TABLE
            
            //cout << "DEST: " << j->getID() << " PORT: " << j->getPort() << " COST: " << j->getCost() << endl;
        }
        i = i->getNext();           // Go to next of their nodes
        j = myNodes->getNext();     // Back to beginning of my nodes
    }
    

}


string BellmanFord::makeDV(RoutingTable r, char nodeLetter) {
    string DV = "~d ";
    string cost;
    Node* myNodes = r.getMyNodes(nodeLetter);   // Get its own routing table
    Node* temp = myNodes->getNext();
    // Format: each line is dest letter, cost (thassit)
    while(temp != NULL) {
        DV += temp->getID();                    // Add dest character
        DV += ",";
        cost = to_string(temp->getCost());
        DV += cost;                             // Add cost (see what the heck this does), may need int to char conversion
        DV += "\n";                             // Newline to separate
        temp = temp->getNext();                 // Go to next in list
    }
    //char* dv_c = (char*)DV.c_str();
    //cout << "dv: " << dv_c << endl;
    return DV;
}


Node* BellmanFord::parseDV(char* buf) {
    NodeList table;
    int cost;
    
    int i = 0;
    while(i < 3){       // Go past initial '~d ", buf[0], [1], [2]
        i++;
    }
    
    // A,3\nB,4\n....etc
    // All the ports are unimportant here. only use dest letter and cost
    Node* head = new Node;
    head->setDest(buf[i]);
    Node* temp = NULL;
    
    //cout << "letter first: " << buf[i] << endl;
    
    while (buf[i] != ',' ) {
        i++;
    }                           // Now it's a comma
    i++;                        // Now it's past the comma
    
    
    cost = buf[i] - '0';        // Convert char to int
    head->setCost(cost);
    
    //cout << "cost first: " << buf[i] << endl;
    
    while (buf[i] != '\n' ) {
        i++;
    }                           // Now it's a newline
    i++;                        // Now it's past newline
    
    Node* prev = head;
    prev->setNext(temp);
    
    // Now get the rest
    while(buf[i] != '\0') {     // While not end of string
        temp = new Node;
        temp->setDest(buf[i]);  // Set dest letter
        
        //cout << "DEST LETTER" << buf[i] << endl;
        
        while (buf[i] != ',' ) {
            i++;
        }                       // Now it's a comma
        i++;                    // Now it's past the comma
        
        
        cost = buf[i] - '0';    // Convert char to int
        temp->setCost(cost);
        
        //cout << "COST" << buf[i] << endl;
        
        prev->setNext(temp);    // Link it to list
        
        prev = temp;            // Get ready to add next node
        while (buf[i] != '\n' ) {
            i++;
        }                       // Now it's a newline
        i++;                    // Now it's past newline
    }
    return head;
}








