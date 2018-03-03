#include <string.h>         // For using strings
#include <iostream>         // For input/ output to console
#include <netinet/in.h>     // For defining router address
#include <stdio.h>          // For perror
#include <arpa/inet.h>      // To use inet_addr for binding
#include <sys/types.h>      // For sockets & binding
#include <sys/socket.h>     // Also sockets & binding


using namespace std;

int main(){
    // Following example from http://matrixsust.blogspot.ie/2011/10/udp-server-client-in-c.html
    
    int sock, bytes_read, addr_len;
    int port_num = 5000;                    // Set the port number
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
    
    int addr_len = sizeof(router_addr);
    if( bind( sock, (struct sockaddr *)&router_addr, addr_len ) == -1 ){    // Bind socket to port
        perror( "Error binding" );
        exit(1);
    }

    cout << "Router online. Accepting data on port " << port_num << endl;
    fflush(stdout);     // Clear output stream

    // Insert code to read and send data...

    return 0;
}