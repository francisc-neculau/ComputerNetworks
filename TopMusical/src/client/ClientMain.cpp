//============================================================================
// Name        : ChessClient.cpp
// Author      : Neculau Francisc
// Version     : 1.0.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <stdlib.h>
#include "string"

using namespace std;

int readConsoleLine(char *, char *);
int charToInt(char *);

static const int PORT = 2728;
static const int SERVER_MESSAGE_LENGTH_BYTES_PROTOCOL = 11;

static const int MAX_CONSOLE_INPUT_LENGTH = 256;

int main() {

//	char * c = "Main Menu -admin- \n1. Show general top \n2. Show custom top \n3. Add song \n4. Delete song \n5. Block user \n";
//	c = (char *)malloc(sizeof(char) * 1);
//
//	cout << sizeof(char) << endl; // 1 byte
//	cout << sizeof(char *) << endl; // 8 bytes
//	cout << sizeof(c) << endl;
//	cout << strlen(c) << endl;
//
//	c[0] = 'b';
//	c[strlen(c)] = '\0';
//
//	cout << sizeof(char) << endl;
//	cout << sizeof(c) << endl;
//	cout << strlen(c) << endl;
//
//	cout << c << endl;
//
//	return 0;

	int serverSocketDescriptor;
	if((serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		cout<<"error";

	struct sockaddr_in serverSocketAddress;
	serverSocketAddress.sin_family      = AF_INET;
	serverSocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverSocketAddress.sin_port        = htons(PORT);

	if(connect(serverSocketDescriptor, (struct sockaddr *) &serverSocketAddress, sizeof(struct sockaddr)) == -1)
		cout << "error";

	/*
	 * Aici clientul incepe comunicarea cu serverul
	 * *** *** *** START *** *** ***
	 */
	char charMessageLength[SERVER_MESSAGE_LENGTH_BYTES_PROTOCOL];
	char consMessage[MAX_CONSOLE_INPUT_LENGTH];
	char * recvMessage;
	char * sendMessage;
	int  length;
	while(1) {
		/*
		 * PRIMIM MESAJUL DE LA SERVER
		 * *** START ***
		 */
			// citim lungimea mesajului
			bzero(charMessageLength, SERVER_MESSAGE_LENGTH_BYTES_PROTOCOL);
			if(recv(serverSocketDescriptor, &charMessageLength, SERVER_MESSAGE_LENGTH_BYTES_PROTOCOL, 0) < 0)
				cout << "error";
			length = charToInt(charMessageLength);

			// alocam memorie
			recvMessage = new char[length];
			bzero(recvMessage, length);

			// citim mesajul propriu zis
			if(recv(serverSocketDescriptor, recvMessage, length, 0) < 0)
				cout << "error";
			// safeguard - terminate the string with '\0'
			recvMessage[strlen(recvMessage)] = '\0';

			// afisam mesajul propriu zis
			cout << recvMessage << endl;

			// verificam daca s-a primit semnalul de inchidere
			if(strcmp(recvMessage, "q") == 0) {
				delete(recvMessage);
				break;
			}

			// dealocam memoria
			delete(recvMessage);
		/*
		 * *** END ***
		 */

		/*
		 * TRIMITEM MESAJUL DE LA CLIENT
		 * *** START ***
		 */
			// formatam variabila ce va incarca mesajul din consola
			bzero(consMessage, MAX_CONSOLE_INPUT_LENGTH);
			bzero(charMessageLength, SERVER_MESSAGE_LENGTH_BYTES_PROTOCOL);

			// citim mesajul de la consola
			length = readConsoleLine("--", consMessage);

			// alocam memorie mesajuli ce v-a fi trimis si copiem informatia in el
			sendMessage = new char[length]; //(char *)malloc(sizeof(char) * length);
			bzero(sendMessage, length);
			strncpy(sendMessage, consMessage, length);
			sendMessage[length] = '\0'; // -> trebuie facut asta deoarece citesc de la tastatura mai mult decat pot duce 256

			// setam lungimea mesajului in char
			snprintf(charMessageLength, sizeof(charMessageLength), "%d", length);

			// trimitem lungimea mesajului in char
			if(send(serverSocketDescriptor, &charMessageLength, SERVER_MESSAGE_LENGTH_BYTES_PROTOCOL, 0) < 0)
				cout << "error";

			// safeguard - terminate the string with '\0'
			sendMessage[strlen(sendMessage)] = '\0';
			// trimitem mesajul propriu-zis
			if(send(serverSocketDescriptor, sendMessage, (sizeof(char) * ((int)strlen(sendMessage))), 0) < 0)
				cout << "error";

			// eliberam memoria
			delete(sendMessage);//free(recvMessage);

		/*
		 * *** END ***
		 */
	}
	/*
	 * *** *** *** END *** *** ***
	 */
	close(serverSocketDescriptor);

	cout << "***END Client***" << endl;
	return 0;
}

int charToInt(char * c) {
	int result = 0, index = 0, digit = 0;

	if(((int)c[index]) == 45) // negative, skip
		index++;

	for (; index < strlen(c); index++) {
		digit = (int)c[index] - 48;
		if(digit > 9)
			break; // should log an error here
		result = result * 10 + digit;
	}

	if((int)c[0] == 45) // negative, set it
		result = -result;

	return result;
}

int readConsoleLine(char * prefix, char * buffer) {
	//printf("@%s: ", prefix);
	fgets(buffer, MAX_CONSOLE_INPUT_LENGTH, stdin);
	/* remove newline, if present */
	if( buffer[strlen(buffer)-1] == '\n')
		buffer[strlen(buffer)-1] = '\0';
	return strlen(buffer);
}
