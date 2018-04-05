#pragma once
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
#include "RoutingTable.h" // may need to update node's table. can you include cpp files?

using namespace std;

class BellmanFord {
public:
    static void bellmanFord(char* buf, RoutingTable r, int clientPort, char nodeLetter);
    static string makeDV(RoutingTable r, char nodeLetter);
    static Node* parseDV(char* buf); //returns head node of newly created nodeList
};

void outputMessage(RoutingTable* routingtable, char vBuff[], char myLetter );