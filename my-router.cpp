#include <string.h>         // For using strings
#include <iostream>         // For input/ output to console
#include <netinet/in.h>     // For defining router address
#include <stdio.h>          // For perror
#include <arpa/inet.h>      // To use inet_addr for binding
#include <sys/types.h>      // For sockets & binding
#include <sys/socket.h>     // Also sockets & binding
#include <stdlib.h>         // Includes atoi func

#include "RoutingTable.cpp"
#include "datagramParser.cpp"

using namespace std;

void broadcastLiveliness( int mySock, Node* myNodes, char myNodeLetter, int my_port_num );

int main(){
    // Following example from http://matrixsust.blogspot.ie/2011/10/udp-server-client-in-c.html
    // Also referred to video by Sloan Kelly; https://www.youtube.com/watch?v=uIanSvWou1M 
    

    // Create routing table and parse the information stored in the file "routingtable.txt"
    RoutingTable routingtable;
    routingtable.makeRoutingTable();
    routingtable.parseData();

    // Read in router infromation from user
    string input;
    char nodeLetter;
    int sock, bytes_read, my_port_num;
    char send_data[1024], recv_data[1024];
    struct sockaddr_in router_addr, client_addr;            
    
    cout << "Enter node letter: ";      // Allow user to enter letter of node being created
    cin >> nodeLetter;
    cout << "Enter portnum: ";          // Allow user to specify the port number of the router being set up
    cin >> input;
    cout << endl;
    my_port_num = atoi( input.c_str() );
    Node* myNodes = routingtable.getMyNodes( nodeLetter );      // Get a pointer to the list of connections this router has
    routingtable.printMyNodes( myNodes );

    // Create and bind socket
    if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 ){    // Set sock to socket descriptor
        perror("Problem opening Socket");                       // using IPv4 and UDP
        exit(1);
    }

    router_addr.sin_family = AF_INET;                           // IPv4
    router_addr.sin_port = htons(my_port_num);                  // Use network byte order
    router_addr.sin_addr.s_addr = inet_addr("127.0.0.1");       // To bind socket to "localhost"
    memset(router_addr.sin_zero, '\0', sizeof(router_addr.sin_zero));       // Clear memory
    
    int addr_len = sizeof(router_addr);
    socklen_t sin_size = sizeof( struct sockaddr );
    if( bind( sock, (struct sockaddr *)&router_addr, addr_len ) == -1 ){    // Bind socket to port
        perror( "Error binding" );
        exit(1);
    }

    cout << "Router " << nodeLetter << " online. Accepting data on port " << my_port_num << "." << endl << endl;
    fflush(stdout);     // Clear output stream

    broadcastLiveliness( sock, myNodes, nodeLetter, my_port_num );

    struct sockaddr_in client;                             // Create client
    int clientLength = sizeof( client );
    socklen_t clientLength2 = sizeof( client );
    char buff[1024];
    int bytes_In;   


    // Router now ready to send and recieve data
    while (1){
        bzero( &client, clientLength );
        bzero( buff, 1024 );

        bytes_In = recvfrom( sock, buff, 1024, 0, (struct sockaddr *)&client, &clientLength2 );
        
        char clientIP[256];
        bzero( clientIP, 256 );

        inet_ntop( AF_INET, &client.sin_addr, clientIP, 256 );          // Change from byte array to chars
        int clientPort = ntohs( client.sin_port );                      // Get port number of sender
        cout << "Message from " << clientIP << ", Port Number " << clientPort <<": " << endl << buff << endl << endl;
        
        
        /* cout << "Send: ";
        cin.getline( send_data, 1024 );

        sendto( sock, send_data, strlen( send_data ), 0, (struct sockaddr *)&client, sizeof(struct sockaddr) ); */

    }

    return 0;
}


void broadcastLiveliness( int mySock, Node* myNodes, char myNodeLetter, int my_port_num ){
    Node* currentNode = myNodes;
    currentNode = currentNode->getNext();
    
    struct sockaddr_in client;
	client.sin_family = AF_INET; // AF_INET = IPv4 addresses
	client.sin_port = htons( currentNode->getPort() ); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &client.sin_addr); // Convert from string to byte array
	
    int clientLength = sizeof( client );
    socklen_t clientLength2 = sizeof( client );
    char buff[1024];
    int bytes_In;

    stringstream message;
    message << "Router " << myNodeLetter << " online. Accepting data on port " << my_port_num << "." << endl;
    const string& temp = message.str();
    const char* cstr = temp.c_str();

    while ( currentNode != NULL ){
        client.sin_port = htons( currentNode->getPort() );
        sendto( mySock, cstr, strlen( cstr ), 0, (struct sockaddr *)&client, sizeof(struct sockaddr) );
        currentNode = currentNode->getNext();
    }
}