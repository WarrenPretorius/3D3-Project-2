#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>         // For using strings
#include <netinet/in.h>     // For defining router address
#include <arpa/inet.h>      // To use inet_addr for binding

using namespace std;

class Node{
private:
    Node* next;
    Node* prev;
    char dest;
    int port;
    int cost;
public:
    Node();
    //accessor and mutator
    Node* getNext(){  return next;}
    Node* getPrev(){    return prev;}
    int getPort(){  return port;}
    int getCost(){  return cost;}
    char getID(){   return dest;}

    void setNext(Node* a){  next = a;}
    void setPrev(Node* a){  prev = a;}
    void setDest(char a){   dest = a;}
    void setPort(int a){ port = a;}
    void setCost(int a){    cost = a;}
};
Node* newNode(char nodeletter){
    Node* a = new Node;
    int port = 10000 + (int)(nodeletter - 65);
    a -> setDest(nodeletter); 
    a -> setPort(port);
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
    NodeList(){ head = NULL;}
    //accessor mutator
    Node* getHead(){ return head;}
    void setHead(Node* a){  head = a;}
};

class RoutingTable{
private:
    int size;
    NodeList* array;
public:
    RoutingTable();  //Make the defualt constructor read in the initial values
    
    void makeRoutingTable();
    void parseData();
    void addEdge(int w, int port, char star, char destination);
    void printTable();
    void updateTable();
    void sendDV();
    void requestDV();
    void route(string filename);
    void createDV();

    NodeList* getArray(int a){   return array + a;}
    int getSize(){  return size;}
};

RoutingTable::RoutingTable(){
    size = 6;
    array = NULL;   //Here the routing tables size can be changed   
}

void RoutingTable::makeRoutingTable(){
    array = (NodeList*)malloc(size * sizeof(NodeList));
    for(int i = 0; i < size; i++){
        array[i].setHead(newNode(char(i + 65)));    //Here I am making every head node in the routing table.
        array[i].setHead(newNode(char(i + 65)));
    } 
}

void RoutingTable::addEdge(int w, int port, char start, char destination){
    
    if(array[start-65].getHead() -> getNext() == NULL){ //If there is no head node the, new node is made the head node
        array[start-65].setHead(newNode(start));
        array[start-65].getHead() -> setNext(newNode(destination));
        array[start-65].getHead() -> getNext() -> setCost(w);
        array[start-65].getHead() -> getNext() -> setPort(port);
        array[start-65].getHead() -> setPrev(NULL); 
        array[start-65].getHead() -> getNext() -> setPrev(array[start-65].getHead());
        return;
    }
    
    Node* temp = array[start-65].getHead();
    while(temp -> getNext() != NULL){   //loops through the linked list untin the end is reached
    temp = temp -> getNext();   //The node is appened to the end of the linked list
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
    
    while (!feof(data)){ //If the EOF is reached, stop reading
        if(a == ',' || a == '\n'){
            a = (char)fgetc(data);  //If the character is  useless, get the next char
        }
        if(a >= 0x41){  //This character is a letter and should be added to the start and destination variables.
            start = a;
            a = fgetc(data);
            while(a < 0x41)
                a = (char)fgetc(data);
            destination = a;
        }
        else{
            if(portnum.length() == 5){  //The portnumber is only 5 characters long
                weight = a;
                stringstream s (weight); //Here I am converting string to int
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

void RoutingTable::createDV(){
    ofstream data;
    data.open("UpdatedRoutingTable.txt");
    
    if(!data.is_open())
        cout << "Not open";
        
    Node* head;
    for(int i = 0; i < size; i++){
    head = array[i].getHead();
    cout << "here";
        while(head != NULL){
            data << head -> getID();
            data << ",";
            data << head -> getNext() -> getID();
            data << ",";
            data << head -> getPort();
            data << "," << head -> getCost();
            data << endl;
            
            head = head -> getNext();
        }
    }
}

/*void RoutingTable::printTable(){
    Node* head;
    for(int i = 0; i < size ; i++){
        cout << i << endl;
        head = array[i].getHead();
        while(head != NULL){
            cout << head -> getID() << "," << head -> getPort() << "," << head -> getCost() << endl;
        }
    }
}*/

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

/*void RoutingTable::updateTable(){
    //Each router should have its own version of the initialisation file.
    //It should then ask its neighbours to send its data with udp messages
    //each router broadcasts a routing table table to all of its neigbourghs
    NodeList* temp;
    temp = routingtable.getArray(0);
    
    for(int i =0; i < routingtable.getSize(); i++){
        if((temp[i].getHead() != NULL) && (this -> getArray(i) -> getHead() == NULL)){
        /*
        Here it is checking that the head of the received linked lists are not NULL. 
        If the received heads are not NULL and the heads of the roiting table on this router are NULL; that means that there are new entries that have been
        Received. They must be added to the routing table on this device (graph)
        */
           // array[i] = temp[i];
        //The table should now be up to date with its neighbours table
       // }
   // }
//}*/

int main(){
    RoutingTable routingtable;
    routingtable.makeRoutingTable();
    routingtable.parseData();
    cout << "about to create dv";
    routingtable.createDV();
    cout<<"Im here";
    
  /*  int sock, bytes_read, addr_len;
    int port_num = routingtable.getArray(0) -> getHead() -> getPort();                    // Set the port number
    char send_data[1024], recv_data[1024];
    struct sockaddr_in router_addr;

    if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 ){    // Set sock to socket descriptor
        perror("Problem opening Socket");                       // using IPv4 and UDP
        exit(1);
    }

    router_addr.sin_family = AF_INET;       // IPv4
    router_addr.sin_port = htons(port_num); // Use network byte order
    router_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   // To bind socket to "localhost"
    memset(router_addr.sin_zero, '\0', sizeof(router_addr.sin_zero));   // Clear memory
    
    addr_len = sizeof(router_addr);
    if( bind( sock, (struct sockaddr *)&router_addr, addr_len ) == -1 ){    // Bind socket to port
        perror( "Error binding" );
        exit(1);
    }

    cout << "Router online. Accepting data on port " << port_num << endl;
    fflush(stdout);     // Clear output stream
    
    return 0;   */
}
