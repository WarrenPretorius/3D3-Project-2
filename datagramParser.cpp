
// UNUSED //


/*
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  

using namespace std;

// This function returns the source port number as an int, a destination port number as an int, and a message as a string that it finds in the
// buff array passed in to it. These can then be passed into another function that sends strings between routers. The buff array must always 
// have to be of the correct format: ~m 10010 10001 message 
// those port numbers are just examples but they need to be of length 5. If you need to change the length, adjust the for loop for source_port & destination_port
void parseMessage(char vBuff[]) {
	char message_type [2];          //this is meant to hold ~m or ~d, stores the first 2 chars in an array, then convert that array to a string
						            //then compare that string to what it should be (ex: ~m or ~d) and if it equals to one of those
							        //then enter the loop for that message type. Have not implemented ~d yet. Also ~m could be improved.
	char source_port[5];            //initial storing of source port number in array, gets converted to int after
	char destination_port[5];       //initial storing of dest port number in array, gets converted to int after
	char message_arr[1009];	        //holds the message, the size is 1024 - the # of chars it takes until we get to the message (15)
	int source_port_num;            //we will convert the array holding the port num to an int
	int dest_port_num;              //we will convert the array holding the dest num to an int
	bool n_ready = true;	        //for initilization

	if (n_ready) {
		for (int i = 0; i < sizeof(message_type); i++) {	//message type will always consist of 2 chars: ~m or ~d
			message_type[i] = vBuff[i];	                    //store the 1st 2 chars of the buffer into the message typr
		}		
	}
	string type(message_type, sizeof(message_type));	    //since array is not null terminated, need to specify length of array

	if (type == "~m") {	                                    //check what type of message we have
		bool n_ready = false;	                            //so we don't initilize again
		for (int j = 3; j < (3 + sizeof(destination_port)); j++) {	    //statr at 3rd character which will contain the dest port num & go till the 8th char
			destination_port[j - 3] = vBuff[j];		                    //start storing the port number from the buffer into the dest port array, j-3 to start at 0 index
		}
		dest_port_num = atoi(destination_port);	                        //convert the array to an int we can use to pass into function & know the dest port number

		for (int k = 9; k < (9 + sizeof(source_port)); k++) {	        //same idea as above
			source_port[k - 9] = vBuff[k];
		}
		source_port_num = atoi(source_port);	                        //same as above, this time for source port number

		for (int z = 15; z <= 50 ; z++) {       //this needs to be altered so it can read in values up until the end of the message. maybe check for a double space so we know we have reached the end of the message. how to implement?
			message_arr[z - 15] = vBuff[z];
		}
		/*for (int y = 0; y < sizeof(message_arr); y++) {
			cout << message_arr[y];
		}
		string message(message_arr, sizeof(message_arr));	//should convert 
		cout << endl << message;	                        //test to see what message is
	}
}

int main(void) {
	char buff[1024] = "~m 10010 10101 this is the message that I am transmitting today";
	//cout << "he\n";
	parseMessage(buff);

	return 0;
}
