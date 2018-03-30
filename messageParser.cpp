#include "messageParser.h"

void messageParser::messageParserCheck(char vBuff[]) {
	bool n_ready = true;									//for initilization

	if (n_ready) {
		for (int i = 0; i < sizeof(message_type); i++) {	//message type will always consist of 2 chars: ~m or ~d
			message_type[i] = vBuff[i];						//store the 1st 2 chars of the buffer into the message typr
		}
	}
	string type(message_type, sizeof(message_type));		//since array is not null terminated, need to specify length of array

	if (type == "~m") {										//check what type of message we have
		bool n_ready = false;								//so we don't initilize again
		messageTypeForward(vBuff);
	}

	if (type == "~d") {
		cout << "\nEnter";
		bool n_ready = false;								//so we don't initilize again
	}

	else {													//small error here, enters this loop even if type == ~m, will fix later
		cout << "\n\nInvalid Type of message in Buffer\n\n";
	}
}

void messageParser::messageTypeForward(char vBuff[]) {
	strcpy(message_arr, vBuff);								//put everyting in vbuff into message arr
	strcpy(message_arr2, vBuff);							//put everyting in vbuff into message arr
	strcpy(message_arr3, vBuff);							//put everyting in vbuff into message arr

	sourceSubString = strtok(message_arr, "S");
	sourceSubString = strtok(NULL, " ");
	source_port_num = atoi(sourceSubString);

	destSubString = strtok(message_arr2, "D");
	destSubString = strtok(NULL, " ");
	dest_port_num = atoi(destSubString);

	substring = strtok(message_arr3, "'");					//find the first '
	substring = strtok(NULL, "'");							//last quote
	string message(substring);								//convert into string that we can send 

			//TEST//
	cout << "Source Port #: " << source_port_num << endl;	//test to see what source port num
	cout << "Dest Port #: " << dest_port_num << endl;		//test to see what dest port num
	cout << endl << message << endl;;						//test to see what message is

	

	//here you would call a function and pass in the above parameters
	//ex: object.sendMessage(string message, int source_port_num, int dest_port_num)
}