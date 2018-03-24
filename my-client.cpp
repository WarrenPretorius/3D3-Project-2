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

    sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(54000); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

	// Socket creation, note that the socket type is datagram
	int out = socket(AF_INET, SOCK_DGRAM, 0);

	// Write out to that socket
	string s = "Hello there buddy old pal";
	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    return 0;
}