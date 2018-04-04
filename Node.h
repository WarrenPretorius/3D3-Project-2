#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

class Node{
private:
    Node* next;
    Node* prev;
    char dest;
    int port;
    int cost;
public:
    Node();
    //accessor and mutator
    
    Node* getNext(){  return next;}
    Node* getPrev(){    return prev;}
    int getPort(){  return port;}
    int getCost(){  return cost;}
    char getID(){   return dest;}

    void setNext(Node* a){  next = a;}
    void setPrev(Node* a){  prev = a;}
    void setDest(char a){   dest = a;}
    void setPort(int a){ port = a;}
    void setCost(int a){    cost = a;}
    
};

//Node* newNode(char nodeletter);