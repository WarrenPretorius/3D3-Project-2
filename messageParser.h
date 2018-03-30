#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

using namespace std;

class messageParser {

private:
	char* substring;	        //will have what is between ' '
	char* sourceSubString;
	char* destSubString;
	char message_arr[1024];	    //holds the everything that is in vBuff so we can take out the message between ' ', the size is 1024, same as vBuff
	char message_arr2[1024];
	char message_arr3[1024];
	int source_port_num;        //we will convert the array holding the port num to an int
	int dest_port_num;          //we will convert the array holding the dest num to an int
	char message_type[2];       //this is meant to hold ~m or ~d, stores the first 2 chars in an array, then convert that array to a string
							    //then compare that string to what it should be (ex: ~m or ~d) and if it equals to one of those
							    //then enter the loop for that message type. Have not implemented ~d yet. Also ~m could be improved.
	bool n_ready = true;	    //for initilization


public:
	void messageParserCheck(char vBuff[]);
	void messageTypeForward(char vBuff[]);
	void messageTypeDV(char vBuff[]);
};