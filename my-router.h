#pragma once
#include <string.h>         // For using strings
#include <iostream>         // For input/ output to console
#include <netinet/in.h>     // For defining router address
#include <stdio.h>          // For perror
#include <arpa/inet.h>      // To use inet_addr for binding
#include <sys/types.h>      // For sockets & binding
#include <sys/socket.h>     // Also sockets & binding
#include <stdlib.h>         // Includes atoi func

#include "RoutingTable.h"
#include "bellmanford.h"

using namespace std;

// Sets up all ports etc for this router
int initialiseRouter( RoutingTable  routingtable, struct sockaddr_in* router, char* nodeLetter );

// Let all my neighbours know I'm alive
void broadcastLiveliness( int mySock, Node* myNodes, char myNodeLetter, int my_port_num );

// Check what type of message was recived and call appropriate secondary parser
void messageParserCheck(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[], RoutingTable routingtable, char myLetter);

// If message type was forward, this is called to forward the message
void messageTypeForward(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]);

// If the message was a DV update, this function gets called to deal with it
void messageTypeDV(char vBuff[]);