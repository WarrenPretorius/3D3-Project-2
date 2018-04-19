// This file is used to test my-router and routing capabilities //

#include <string.h>         // For using strings
#include <iostream>         // For input/ output to console
#include <netinet/in.h>     // For defining router address
#include <stdio.h>          // For perror
#include <arpa/inet.h>      // To use inet_addr for binding
#include <sys/types.h>      // For sockets & binding
#include <sys/socket.h>     // Also sockets & binding
#include <stdlib.h>         // Includes atoi func
#include <sstream>			// To allow use of say, message << "Hello " << name << endl;

using namespace std;

int main(){

    sockaddr_in server;									// Create server / router to send dataframs to
	server.sin_family = AF_INET; 						// AF_INET = IPv4 addresses
	server.sin_port = htons(10000); 					// Little to big endian conversion, with port number
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); 	// Convert from string to byte array
	
	sockaddr_in client;                             // Create client
    int clientLength = sizeof( client );			// Hold the size of the sockaddr_in struct
    socklen_t clientLength2 = sizeof( client );		// Need this version of sockaddr_in size for recvfrom
    char buff[1024], send_data[1024];				// Char arrays to hold the messages we're sending and receiving
    int bytes_In;									// Used for receiving messages, recvfrom

	char messageType;					// Variable to hold message type entered by user
	char finalDestPortChar[1024];		// Variable to hold the final destination of the datagram
	char injectedPortChar[1024];		// Variable to hold the port at which to inject a packet
	char viaPortChar[1024];				// Variable to hold which port the packet should be routed via
	int finalDestPort;					// Int version of finalDestPortChar
	int injectedPort;					// Int version of injectedPortChar
	int viaPort;						// Int version of viaPortCahr

	while (true){
		bzero( &client, clientLength );		// Clear the client variable
		bzero( buff, 1024 );				// CLear the input buffer

		// Socket creation, note that the socket type is datagram
		int out = socket(AF_INET, SOCK_DGRAM, 0);

		// Ask user for inputs to send messages between routers
		cout << "Enter D for DV message or M for normal message or F for forwarding: ";
		cin >> messageType;
		cin.getline( finalDestPortChar, 1024 );					//Clear input stream

		if (messageType == 'D'){						// If the message is a Distance vector
			stringstream message_send;
			message_send << "~d A,3\nC,3\nE,2\nF,1\n";	// Hardcode an update from A to B
			const string& temp = message_send.str();	// Convert stringstream to a string
			const char* cstr = temp.c_str();			// Convert string to cstr
			sendto( out, cstr, strlen( cstr ), 0, (sockaddr*)&server, sizeof(server) );	// Send DV update to A, 10000

		}
		else if ( messageType == 'F'){							// If we want to test forwarding
			cout << "Enter final destination port: ";			// Ask user for end address of datagram 
			cin.getline( finalDestPortChar, 1024 );				// Save entry
			cout << "Enter port at which to inject packet: ";	// Ask user where to inject the packet
			cin.getline( injectedPortChar, 1024 );				// Save entry
			cout << "Enter port to send packet via: ";			// Ask which port to send it by
			cin.getline( viaPortChar, 1024 );					// Save entry
			cout << endl;

			finalDestPort = atoi( finalDestPortChar );	// Convert ascii entry into an integer
			injectedPort = atoi( injectedPortChar );	// Convert ascii entry into an integer
			viaPort = atoi( viaPortChar );				// Convert ascii entry into an integer

			stringstream message_send;
			// Construct message to be sent..
			message_send << "~m S" << injectedPort << " D" << finalDestPort << " 'Testing message forwarding'" << endl;
			const string& temp = message_send.str();	// Convert ss to string
			const char* cstr = temp.c_str();			// COnvert string to cstr

			server.sin_port = htons( viaPort );			// Update the server/ router port to the via port
			sendto(out, cstr, strlen( cstr ), 0, (sockaddr*)&server, sizeof(server));	// Send the message via the 'viaPort'
		}
		else{												// If not a DV update or Forwarding it must just be a normal 1 to 1 message
			cout << "Enter desination port: ";				// Where are we sending the message?
			cin.getline( finalDestPortChar, 1024 );			// Save user entry
			finalDestPort = atoi( finalDestPortChar );		// Convert ascii ti integer
			cout << finalDestPort << endl;
			cout << "Enter message: ";						// What (text) message do you want to send?
			cin.getline( send_data, 1024 );					// Save entry

			string message(send_data);						// Convert char array to string
			stringstream message_send;						// Create ss variable to build message
			// Construct datagram meassage
			message_send << "~m S" << client.sin_port << " D" << finalDestPort << " '" <<message << "'" << endl;
			const string& temp = message_send.str();		// Convert ss to string
			const char* cstr = temp.c_str();				// Convert string to cstr

			server.sin_port = htons( finalDestPort );		// Update destination port
			sendto(out, cstr, strlen( cstr ), 0, (sockaddr*)&server, sizeof(server));	// Send datagram
		}


		// Wait to receive a message
		/*
		bytes_In = recvfrom( out, buff, 1024, 0, (struct sockaddr *)&client, &clientLength2 );
		
		char clientIP[256];
		bzero( clientIP, 256 );

		inet_ntop( AF_INET, &client.sin_addr, clientIP, 256 );          // Change from byte array to chars
		int clientPort = ntohs( client.sin_port );						// Get port number of sender
        cout << "Message from " << clientIP << " , Port Number " << clientPort <<" : " << buff << endl;
		cout << "Message from " << clientIP << " : " << buff << endl;
		*/

	}
	

    return 0;
}