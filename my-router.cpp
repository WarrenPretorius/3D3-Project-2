#include "my-router.h"

int main(){

    // Create routing table and parse the information stored in the file "routingtable.txt"
    RoutingTable routingtable;
    routingtable.makeRoutingTable();
    routingtable.parseData();

    // Set up router with socket and broadcast liveliness    
    char nodeLetter;
    struct sockaddr_in router;
    int sock = initialiseRouter( routingtable, &router, &nodeLetter );
    
    struct sockaddr_in client;                             // Create client
    int clientLength = sizeof( client );
    socklen_t clientLength2 = sizeof( client );
    char buff[1024];
    int bytes_In;   


    // Router now ready to send and recieve data
    while (1){
        
        bzero( &client, clientLength );
        bzero( buff, 1024 );

        bytes_In = recvfrom( sock, buff, 1024, 0, (struct sockaddr *)&client, &clientLength2 );
        int clientPort = ntohs( client.sin_port );
        int my_port = ntohs( router.sin_port );
        
        messageParserCheck( sock, my_port, routingtable.getMyNodes(nodeLetter), client, buff );
        
        
        //char clientIP[256];
        //bzero( clientIP, 256 );

        //inet_ntop( AF_INET, &client.sin_addr, clientIP, 256 );          // Change from byte array to chars
        //int clientPort = ntohs( client.sin_port );                      // Get port number of sender
        //cout << "Message from " << clientIP << ", Port Number " << clientPort <<": " << endl << buff << endl << endl;

    }

    return 0;
}


int initialiseRouter( RoutingTable  routingtable, struct sockaddr_in* router, char* nodeLetter ) {

    string input;
    int sock, my_port_num;
    //struct sockaddr_in router_addr;

    cout << "Enter node letter: ";      // Allow user to enter letter of node being created
    cin >> *nodeLetter;
    cout << "Enter portnum: ";          // Allow user to specify the port number of the router being set up
    cin >> input;
    cout << endl;
    my_port_num = atoi( input.c_str() );
    Node* myNodes = routingtable.getMyNodes( *nodeLetter );      // Get a pointer to the list of connections this router has
    routingtable.printMyNodes( myNodes );

    // Create and bind socket
    if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 ){    // Set sock to socket descriptor
        perror("Problem opening Socket");                       // using IPv4 and UDP
        exit(1);
    }

    (*router).sin_family = AF_INET;                           // IPv4
    (*router).sin_port = htons(my_port_num);                  // Use network byte order
    (*router).sin_addr.s_addr = inet_addr("127.0.0.1");       // To bind socket to "localhost"
    memset( (*router).sin_zero, '\0', sizeof( (*router).sin_zero));       // Clear memory
    
    int addr_len = sizeof(*router);
    socklen_t sin_size = sizeof( struct sockaddr );
    if( bind( sock, (struct sockaddr *)&(*router), addr_len ) == -1 ){    // Bind socket to port
        perror( "Error binding" );
        exit(1);
    }

    cout << "Router " << *nodeLetter << " online. Accepting data on port " << my_port_num << "." << endl << endl;
    fflush(stdout);     // Clear output stream

    broadcastLiveliness( sock, myNodes, *nodeLetter, my_port_num );

    return sock;
}

void broadcastLiveliness( int mySock, Node* myNodes, char myNodeLetter, int my_port_num ){
    Node* currentNode = myNodes;
    currentNode = currentNode->getNext();
    
    struct sockaddr_in client;
	client.sin_family = AF_INET; // AF_INET = IPv4 addresses
	client.sin_port = htons( currentNode->getPort() ); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &client.sin_addr); // Convert from string to byte array

    stringstream message;
    
    cout << "Sending following messages: " << endl;
    while ( currentNode != NULL ){
        int clientPort = currentNode->getPort();
        client.sin_port = htons( clientPort );

        message.str("");
        message << "~m S" << my_port_num << " D"<< clientPort << " 'Router " << myNodeLetter << " online. Accepting data on port " << my_port_num << ".'" << endl;
        cout <<"~m S" << my_port_num << " D"<< clientPort << " 'Router " << myNodeLetter << " online. Accepting data on port " << my_port_num << ".'" << endl;
        const string& temp = message.str();
        const char* cstr = temp.c_str();

        sendto( mySock, cstr, strlen( cstr ), 0, (struct sockaddr *)&client, sizeof(struct sockaddr) );
        currentNode = currentNode->getNext();
    }
    cout << endl;
}

void messageParserCheck(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]) {

	char message_type[2];       // This is meant to hold ~m or ~d, stores the first 2 chars in an array, then convert that array to a string
							    // then compare that string to what it should be (ex: ~m or ~d) and if it equals to one of those
							    // then enter the loop for that message type. Have not implemented ~d yet. Also ~m could be improved.
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

void messageTypeForward(int my_sock, int my_port, Node* my_nodes, sockaddr_in client, char vBuff[]) {
	
    int dest_port_num;          // We will convert the array holding the port num to an int
    int source_port_num;        
    char* substring;            // Will have what is between ' '
    char* destSubString;
    char* sourceSubString;
    char message_arr[1024];	    // Holds the everything that is in vBuff so we can take out the message between ' ', the size is 1024, same as vBuff
	char message_arr2[1024];
	char message_arr3[1024];

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

    //cout << "Message Recieved... " << endl;
    //cout << "Source Port: " << source_port_num << endl;
    //cout << "Dest Port: " << dest_port_num << endl;

    stringstream message_send;
    message_send << "~m S" << source_port_num << " D"<< dest_port_num << " '" << message << "'" << endl;
    const string& temp = message_send.str();
    const char* cstr = temp.c_str();

    if (dest_port_num == my_port){
        //cout << "In if statement " << endl;
        cout << "Message from " << source_port_num << ": " << message << endl;
    }
    else {
        //cout << "In else statement " << endl;
        Node* currentNode = my_nodes;
        currentNode = currentNode->getNext();

        while (currentNode != NULL){
            if (currentNode->getPort() == dest_port_num){
                //const char* cstr = message.c_str();
                client.sin_port = htons( dest_port_num );
                sendto( my_sock, cstr, strlen( cstr ), 0, (struct sockaddr *)&client, sizeof(struct sockaddr) );
                cout << "Message sent to " << dest_port_num << endl;
            }
            currentNode = currentNode->getNext();
        }
        
    }
}


/* void outputMessage(RoutingTable* routingtable, char[] vBuff){
    ofstream data;
    data.open("router-output.txt");
    
    if(!data.is_open())
        cout << "Not open";
    
    if(vBuff[0]=='~' && vBuff[1]=='D'){    
    Node* head;
    Node* temp;
    for(int i = 0; i < size; i++){
    head = routingtable -> getArray(i).getHead();
    temp = routingtable -> getArray(i).getHead();
        while(temp -> getNext() != NULL){
            data << head -> getID();
            data << ",";
            data << temp -> getNext() -> getID();
            data << ",";
            data << temp -> getNext() -> getPort();
            data << "," << temp -> getNext() -> getCost() << endl;
            
            temp = temp -> getNext();
        }        
    }
    }
    
    else{
        for(int i = 0; i<sizeof[vBuff]; i++){
            data << vBuff[i];
        }
    }
    
} */