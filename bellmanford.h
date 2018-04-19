#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "RoutingTable.h"

using namespace std;

class BellmanFord {
public:
    void bellmanFord(char* buf, RoutingTable r, int clientPort, char nodeLetter);
    string makeDV(RoutingTable r, char nodeLetter);
    Node* parseDV(char* buf);   // Returns head node of newly created nodeList
};