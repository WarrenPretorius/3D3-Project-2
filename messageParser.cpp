#include "messageParser.h"
#include "my-router.h"

void messageParser::messageParserCheck(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]) {
	
    bool n_ready = true;									// For initilization

	if (n_ready) {
		for (int i = 0; i < sizeof(message_type); i++) {	// Message type will always consist of 2 chars: ~m or ~d
			message_type[i] = vBuff[i];						// Store the 1st 2 chars of the buffer into the message typr
		}
	}
	string type(message_type, sizeof(message_type));		// Since array is not null terminated, need to specify length of array

	if (type == "~m") {										// Check what type of message we have
		bool n_ready = false;								// So we don't initilize again
		messageTypeForward(my_sock, my_port, my_nodes, client, vBuff);
	}

	else if (type == "~d") {
		cout << "\nEnter";
		bool n_ready = false;								// So we don't initilize again
	}

	else {													// Small error here, enters this loop even if type == ~m, will fix later
		cout << "\n\nInvalid Type of message in Buffer\n\n";
	}
}

void messageParser::messageTypeForward(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]) {
	
    strcpy(message_arr, vBuff);								// Put everyting in vbuff into message arr
	strcpy(message_arr2, vBuff);							// Put everyting in vbuff into message arr2
	strcpy(message_arr3, vBuff);							// Put everyting in vbuff into message arr3

	sourceSubString = strtok(message_arr, "S");
	sourceSubString = strtok(NULL, " ");
	source_port_num = atoi(sourceSubString);

	destSubString = strtok(message_arr2, "D");
	destSubString = strtok(NULL, " ");
	dest_port_num = atoi(destSubString);

	substring = strtok(message_arr3, "'");					// Find the first '
	substring = strtok(NULL, "'");							// Last quote
	string message(substring);								// Convert into string that we can send

    if (dest_port_num == my_port){
        cout << "Message from " << source_port_num << ": " << message << endl;
    }
    else {
        Node* currentNode = my_nodes;
        currentNode = currentNode->getNext();

        while (currentNode != NULL){
            if (currentNode->getPort == dest_port_num){
                const char* cstr = message.c_str();
                client.sin_port = htons( dest_port_num );
                sendto( my_sock, cstr, strlen( cstr ), 0, (struct sockaddr *)&client, sizeof(struct sockaddr) );
            }
        }
        
    }

			//TEST//
	//cout << "Source Port #: " << source_port_num << endl;	// Test to see what source port num
	//cout << "Dest Port #: " << dest_port_num << endl;		// Test to see what dest port num
	//cout << endl << message << endl;;						// Test to see what message is

	

	//here you would call a function and pass in the above parameters
	//ex: object.sendMessage(string message, int source_port_num, int dest_port_num)
}