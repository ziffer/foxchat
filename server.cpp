/*
We will create an im server.
Right now, when someone connects,
it only returns a string.

To test, run the application,
open your browser got to your ip and port.


*/




#include <iostream>
#include <vector>
#include <string>
//Needed for network
#include <cstdlib>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
// Needed for OS X
#include <unistd.h>

using namespace std;

//Structure containing username and ip
typedef struct User {
  string name;
	int ip;
} User;

void addUser(int socket);

int main(int argc, char** argv) {
	/*
	Verifies that we have at least 2 argument.
	The first argument (argv[0]) is the name of the binary
	*/
	if (argc < 2) {
		cout << "USAGE: " << argv[0] << " <port>" << endl;
		return 1; //Exit if we don't have the enough arguments
	}


	/*
	To store all the users that are available, I decided to
	put them in a vector of our created type User so we can
	easily remove or add to the vector without worrying about
	pointers.
	*/
	vector<User> users;


	/*
	To create a socket that will allow us to connect / get connected
	we first need a File Descriptor for the server and a second
	to bind to client
	*/
	int serverSocketFileDescriptor, clientSocketFileDescriptor, portno;

	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;


	/*
	To manage the server's process, we will create an integer
	containing the process id
	*/
	int pid;
	


	




	/*
	Now It's time to initialise everyting!
	We will start by creating the server socket...
	*/
	serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	//We verify that it was successfuly created...
	if (serverSocketFileDescriptor < 0) {
		cout << "ERROR creating socket..." << endl;
		return 1;
	}

	/*
	Now we need to write zeros all over serv_addr
	*/
	//bzero((char *) &serv_addr, sizeof(serv_addr));


	/*
	Since our the port specified in our program argument
	is stored in a character array, we need to convert it
	to an integer
	*/
	portno = atoi(argv[1]);


	/*
	Remember our structure serv_addr?
	Well now we have to fill in the variables
	*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);


	/*
	Now it's time to bind our File Descriptors together
	If we fail, we quit.
	*/
	if (bind(serverSocketFileDescriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		cout << "ERROR on binding" << endl;
		return 1;
	}


	listen(serverSocketFileDescriptor, 5);
	clilen = sizeof(cli_addr);

	/*
	The program will now run in an endless loop
	*/
	while (1) {
		//We Accept the connection
		clientSocketFileDescriptor = accept(serverSocketFileDescriptor, (struct sockaddr *)&cli_addr, &clilen);
		//We verify that it was well accepted
		if (clientSocketFileDescriptor < 0) {
			cout << "ERROR accepting client" << endl;
		}

		pid = fork();
		if (pid < 0) {
			cout << "ERROR on fork" << endl;
		}
		if (pid == 0)  {
			close(serverSocketFileDescriptor);
			addUser(clientSocketFileDescriptor);
			exit(0);
		}
		else {
			close(clientSocketFileDescriptor);
		}
	}

	//Now we finish
	close(serverSocketFileDescriptor);
	return 0;
}


void addUser(int socket) {
	int n;
	/*
	Our buffer contains the message that are coming in and out
	of the server. But first we have to write 0s all over it.
	*/
	char buffer[256];
	//bzero(buffer, 256);

	/*
	Now we're going to try to read the message...
	*/
	n = read(socket, buffer, 255);
	if (n < 0) {
		cout << "ERROR reading from socket" << endl;
		return;
	}
	cout << "The server received:" << endl;
	cout << buffer << endl;

	/*
	Now we're going to try to write to the user
	*/
	n = write(socket, "What's up bro?!", 15);
	if (n < 0) {
		cout << "ERROR writing to socket" << endl;
	}
}
