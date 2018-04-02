//
//  bellmanford.h
//  
//
//  Created by Sophia Rosney on 31/03/2018.
//
//

#ifndef bellmanford_h
#define bellmanford_h


#endif /* bellmanford_h */

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
#include "RoutingTable.cpp" // may need to update node's table. can you include cpp files?
#include "my-router.h"

using namespace std;

class BellmanFord {
public:
    void bellmanFord(char* buf, RoutingTable r, int clientPort, char nodeLetter);
    string makeDV(RoutingTable r, char nodeLetter);
    Node* parseDV(char* buf); //returns head node of newly created nodeList
};
