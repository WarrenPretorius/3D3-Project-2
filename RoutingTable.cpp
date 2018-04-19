#include "RoutingTable.h"

// Function to create new Node for the graph
Node* newNode(char nodeletter){
    Node* a = new Node;
    int port = 10000 + (int)(nodeletter - 65);
    a -> setDest(nodeletter); 
    a -> setPort(port);
    return a;
}

// Constructor for our RoutingTable class
RoutingTable::RoutingTable(){
    size = 6;       // Number of Nodes we'll have in graph
    array = NULL;   // Set the array of Nodes to NULL at start
}

// Function to construct our Routing Table which is an array of pointers
void RoutingTable::makeRoutingTable(){
    array = (NodeList*)malloc(size * sizeof(NodeList));
    for(int i = 0; i < size; i++){
        array[i].setHead(newNode(char(i + 65)));    // Making every head node in the routing table.
    } 
}

// Function to update the cost and destination port of one Node to another, i.e. their edge
void RoutingTable::addEdge(int w, int port, char start, char destination){
    
    if( array[start-65].getHead() -> getNext() == NULL ){   // If there is no head node, the new node is made the head node
        array[start-65].setHead( newNode(start) );
        array[start-65].getHead() -> setNext( newNode(destination) );
        array[start-65].getHead() -> getNext() -> setCost(w);
        array[start-65].getHead() -> getNext() -> setPort(port);
        array[start-65].getHead() -> setPrev( NULL ); 
        array[start-65].getHead() -> getNext() -> setPrev( array[start-65].getHead() );
        return;
    }
    
    Node* temp = array[start-65].getHead();
    while(temp -> getNext() != NULL){           // Loops through the linked list until the end is reached
    temp = temp -> getNext();                   // The node is appended to the end of the linked list
    }
    
    Node* end = newNode(destination);
    temp -> setNext(end);
    end -> setPrev(temp);
    end -> setCost(w);
    end -> setPort(port);
}

// Function to take the data from the routing table text file and creates the graph from this
void RoutingTable::parseData(){
    FILE* data;
    data = fopen("routingtable.txt", "r");
    
    if( data == NULL )
        cout << "Data is NULL \n";
        
    int n = 2;
    string portnum;
    string buffer;
    string weight;
    char start, destination;
    int w = 0, p = 0;
    char a = (char) fgetc(data);
    
    while (!feof(data)){                // If the EOF is reached, stop reading
        if(a == ',' || a == '\n'){
            a = (char)fgetc(data);      // If the character is  useless, get the next char
        }
        if(a >= 0x41){                  // This character is a letter and should be added to the start and destination variables.
            start = a;
            a = fgetc(data);
            while(a < 0x41)
                a = (char)fgetc(data);
            destination = a;
        }
        else{
            if(portnum.length() == 5){      // The portnumber is only 5 characters long
                weight = a;
                stringstream s (weight);    // Converting string to int
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

// Function to take a DV message to update a table
void RoutingTable::parseData(string line){
    char dest, start;
    int w = 0, p = 0;
    dest = line[2];
    start = line[0];
    stringstream s1 (line.substr(4,8));
    w = line[10] - 65;
    s1 >> p;
    addEdge(w, p, start, dest);    
}

// Update our routing table file
void RoutingTable::createDV(){
    ofstream data;
    data.open("UpdatedRoutingTable.txt");
    
    if(!data.is_open())
        cout << "Not open";
        
    Node* head;
    Node* temp;
    data << "~D" << endl;
    for(int i = 0; i < size; i++){
    head = array[i].getHead();
    temp = array[i].getHead();
        while(temp -> getNext() != NULL){
            data << head -> getID();
            data << ",";
            data << temp -> getNext() -> getID();
            data << ",";
            data << temp -> getNext() -> getPort();
            data << "," << temp -> getNext() -> getCost() << endl;
            
            temp = temp -> getNext();
        }
    }
}

// Print to console the whole routing table
void RoutingTable::printTable(){
    Node* head;
    for(int i = 0; i < size ; i++){
        head = array[i].getHead();
        while(head != NULL){
            cout << head -> getID() << "," << head -> getPort() << "," << head -> getCost() << endl;
            head = head -> getNext();
        }
        cout << endl;
    }
    cout << endl << endl;
}

void RoutingTable::requestDV(){
    /*
    This functions role is to loop through the current routing table and to send a request to all of its direct neighbours.
    The request will be for its routing table. The request will be a UDP packet. It should be sent periodically. The id number of the packet should be
    added to the packet
    */
    
}

void RoutingTable::sendDV(){
    /*
    This function will have to send its routing table to a router upon request.
    The Request should have an ID associated with it so that the router knows where to send the routing table to.
    */
    
}

void RoutingTable::updateTable(){
    /*
    The router receives a dv message and it then takes that message and parses it into its own routing table.
    The router must avoid putting in things that it already has
    */
    fstream data;
    data.open("UpdatedRoutingTable.txt");
    
    string line;
    string dvmessage;
    if (data.is_open()){
        while ( getline (data,line) ){
            dvmessage = dvmessage + line + "\n";
        }
        data.close();
    }
    
    for( int i = 0; i < dvmessage.length(); i++ ){
        if( dvmessage[i] <= 'Z' && dvmessage[i] >= 'A' ){
            if( dvmessage[i+2] <= 'Z' && dvmessage[i+2] >= 'A' ){
                if( array + ( dvmessage[i]-65)  != NULL ){
                    parseData( dvmessage.substr(i,i+10) );
                    i = i + 12;
                }
            }
        }
    }
}

// Function to return the Nodes this router is connected to
Node* RoutingTable::getMyNodes( char myLetter ){
    Node* myNodes;
    for (int i = 0; i < size; i++ ){
        myNodes = array[i].getHead();
        if ( myNodes->getID() == myLetter ){
            return myNodes;
        }
    }
    return NULL;
}

// Function to print the list of NOdes this Router is connected to
void RoutingTable::printMyNodes( Node* myNodes ){
    Node* currentNode = myNodes;
    cout << "My connections are: " << endl;
    while(currentNode != NULL){
            cout << currentNode -> getID() << ", " << currentNode -> getPort() << ", " << currentNode -> getCost() << endl;
            currentNode = currentNode -> getNext();
        }
        cout << endl;
}