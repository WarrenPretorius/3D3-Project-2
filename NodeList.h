#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include "Node.h"

using namespace std;

class NodeList{
private:
    Node* head;
public:
    NodeList(){ head = NULL;}
    //accessor mutator
    Node* getHead(){ return head;}
    void setHead(Node* a){  head = a;}
};
