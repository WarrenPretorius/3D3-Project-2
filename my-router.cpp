#include "RoutingTable.cpp"
#include "my-router.h"

int main(){

    // Create routing table and parse the information stored in the file "routingtable.txt"
    RoutingTable routingtable;
    routingtable.makeRoutingTable();
    routingtable.parseData();

    // Set up router with socket and broadcast liveliness    
    char nodeLetter;
    struct sockaddr_in router;
    int sock = initialiseRouter( routingtable, &router, &nodeLetter );
    
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
        int clientPort = ntohs( client.sin_port );
        
        
        messageParserCheck( sock, clientPort, routingtable.getMyNodes(nodeLetter), client, buff );
        
        
        //char clientIP[256];
        //bzero( clientIP, 256 );s

        //inet_ntop( AF_INET, &client.sin_addr, clientIP, 256 );          // Change from byte array to chars
        //int clientPort = ntohs( client.sin_port );                      // Get port number of sender
        //cout << "Message from " << clientIP << ", Port Number " << clientPort <<": " << endl << buff << endl << endl;

    }

    return 0;
}