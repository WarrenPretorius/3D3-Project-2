#include <string.h>         // For using strings
#include <iostream>         // For input/ output to console
#include <netinet/in.h>     // For defining router address
#include <stdio.h>          // For perror
#include <arpa/inet.h>      // To use inet_addr for binding
#include <sys/types.h>      // For sockets & binding
#include <sys/socket.h>     // Also sockets & binding
#include <stdlib.h>         // Includes atoi func
#include <sstream>

using namespace std;

int main(){

    sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(10000); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array
	
	sockaddr_in client;                             // Create client
    int clientLength = sizeof( client );
    socklen_t clientLength2 = sizeof( client );
    char buff[1024], send_data[1024];
    int bytes_In;

	char messageType;
	char finalDestPortChar[1024];
	char injectedPortChar[1024];
	char viaPortChar[1024];
	int finalDestPort;
	int injectedPort;
	int viaPort;

	while (true){
		bzero( &client, clientLength );
		bzero( buff, 1024 );

		// Socket creation, note that the socket type is datagram
		int out = socket(AF_INET, SOCK_DGRAM, 0);

		// Ask user for inputs to send messages between routers

		cout << "Enter D for DV message or M for normal message: ";
		cin >> messageType;
		cin.getline( finalDestPortChar, 1024 );					//Clear input stream

		if (messageType == 'D'){
			//hardcode a message from A to B
			stringstream message_send;
			message_send << "~d A,3\nC,3\nE,2\nF,1\n";

			const string& temp = message_send.str();
			const char* cstr = temp.c_str();
			sendto( out, cstr, strlen( cstr ), 0, (sockaddr*)&server, sizeof(server) );

		}
		else if ( messageType == 'M'){
			cout << "Enter final destination port: ";
			cin.getline( finalDestPortChar, 1024 );
			cout << "Enter port at which to inject packet: ";
			cin.getline( injectedPortChar, 1024 );
			cout << "Enter port to send packet via: ";
			cin.getline( viaPortChar, 1024 );
			cout << endl;

			finalDestPort = atoi( finalDestPortChar );
			injectedPort = atoi( injectedPortChar );
			viaPort = atoi( viaPortChar );

			stringstream message_send;
			message_send << "~m S" << injectedPort << " D" << finalDestPort << " 'Testing message forwarding'" << endl;
			const string& temp = message_send.str();
			const char* cstr = temp.c_str();
			sendto(out, cstr, strlen( cstr ), 0, (sockaddr*)&server, sizeof(server));
		}

		/*
		bytes_In = recvfrom( out, buff, 1024, 0, (struct sockaddr *)&client, &clientLength2 );
		
		char clientIP[256];
		bzero( clientIP, 256 );

		inet_ntop( AF_INET, &client.sin_addr, clientIP, 256 );          // Change from byte array to chars
		int clientPort = ntohs( client.sin_port );						// Get port number of sender
        cout << "Message from " << clientIP << " , Port Number " << clientPort <<" : " << buff << endl; */
		//cout << "Message from " << clientIP << " : " << buff << endl;
	}
	

    return 0;
}