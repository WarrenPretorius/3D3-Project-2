#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include "Node.h"
#include "NodeList.h"

using namespace std;


class RoutingTable{
private:
    int size;
    NodeList* array;
public:
    RoutingTable();  //Make the defualt constructor read in the initial values
    
    void makeRoutingTable();
    void parseData();
    void parseData(string line);
    void addEdge(int w, int port, char star, char destination);
    void printTable();
    void updateTable();
    void sendDV();
    void requestDV();
    void route(string filename);
    void createDV();
    Node* getMyNodes( char myLetter );
    void printMyNodes( Node* myNodes );

    NodeList* getArray(int a){   return array + a;}
    int getSize(){  return size;}
};

