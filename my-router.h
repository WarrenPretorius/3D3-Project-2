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

int initialiseRouter( RoutingTable  routingtable, struct sockaddr_in* router, char* nodeLetter );
void broadcastLiveliness( int mySock, Node* myNodes, char myNodeLetter, int my_port_num );
void messageParserCheck(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]);
void messageTypeForward(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]);
void messageTypeDV(char vBuff[]);
//void outputMessage(RoutingTable* routingtable, char[] vBuff);