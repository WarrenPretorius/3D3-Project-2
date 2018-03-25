#include <string.h>         // For using strings
#include <iostream>         // For input/ output to console
#include <netinet/in.h>     // For defining router address
#include <stdio.h>          // For perror
#include <arpa/inet.h>      // To use inet_addr for binding
#include <sys/types.h>      // For sockets & binding
#include <sys/socket.h>     // Also sockets & binding
#include <stdlib.h>         // Includes atoi func

using namespace std;

int main(){
    // Following example from http://matrixsust.blogspot.ie/2011/10/udp-server-client-in-c.html
    
    
    int sock, bytes_read;
    int port_num;
    char send_data[1024], recv_data[1024];
    struct sockaddr_in router_addr, client_addr;

    string input;                           
    cout << "What's my portnum: ";          // Allow user to specify the port number of the router being set up
    cin >> input;
    port_num = atoi( input.c_str() );

    if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 ){    // Set sock to socket descriptor
        perror("Problem opening Socket");                       // using IPv4 and UDP
        exit(1);
    }

    router_addr.sin_family = AF_INET;                           // IPv4
    router_addr.sin_port = htons(port_num);                     // Use network byte order
    router_addr.sin_addr.s_addr = inet_addr("127.0.0.1");       // To bind socket to "localhost"
    memset(router_addr.sin_zero, '\0', sizeof(router_addr.sin_zero));       // Clear memory
    
    int addr_len = sizeof(router_addr);
    socklen_t sin_size = sizeof( struct sockaddr );
    if( bind( sock, (struct sockaddr *)&router_addr, addr_len ) == -1 ){    // Bind socket to port
        perror( "Error binding" );
        exit(1);
    }

    cout << "Router online. Accepting data on port " << port_num << endl;
    fflush(stdout);     // Clear output stream

    struct sockaddr_in client;                             // Create client
    int clientLength = sizeof( client );
    socklen_t clientLength2 = sizeof( client );
    char buff[1024];
    int bytes_In;
    
   

    while (1){
        bzero( &client, clientLength );
        bzero( buff, 1024 );

        bytes_In = recvfrom( sock, buff, 1024, 0, (struct sockaddr *)&client, &clientLength2 );
        
        char clientIP[256];
        bzero( clientIP, 256 );

        inet_ntop( AF_INET, &client.sin_addr, clientIP, 256 );          // Change from byte array to chars
        int clientPort = ntohs( client.sin_port );                      // Get port number of sender
        cout << "Message from " << clientIP << " , Port Number " << clientPort <<" : " << buff << endl;
        
        
        cout << "Send: ";
        cin.getline( send_data, 1024 );

        sendto( sock, send_data, strlen( send_data ), 0, (struct sockaddr *)&client, sizeof(struct sockaddr) );

    }

    return 0;
}