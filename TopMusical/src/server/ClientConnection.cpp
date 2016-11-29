/*
 * Client.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: francisc
 */

#include "ClientConnection.h"

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector"

using namespace std;


ClientConnection::ClientConnection(	struct sockaddr_in socketAddress, int socketDescriptor) {
	this->logger = new Logger();
	this->socketAddress    = socketAddress;
	this->socketDescriptor = socketDescriptor;
}

bool ClientConnection::sendMessage(const char * message) {
	char * sendMessage;
	/*
	 * 1. Send length of message
	 */
	char charMessageLength[11];
	bzero(charMessageLength, 11);
	snprintf(charMessageLength, sizeof(charMessageLength), "%d", (int)strlen(message) + 1);

	if(send(this->socketDescriptor, charMessageLength, sizeof(charMessageLength), 0) < 0)
		return false;
	/*
	 * 2. Send actual message
	 */
	// SAFEGUARD - terminate the string with '\0'
	sendMessage = new char[strlen(message)+1];
	bzero(sendMessage, strlen(message)+1);
	strncpy(sendMessage, message, strlen(message));
	sendMessage[strlen(message)] = '\0';

	if(send(this->socketDescriptor, sendMessage, (sizeof(char) * ((int)strlen(message) + 1)), 0) < 0)
		return false;

	return true;
}
bool ClientConnection::sendMessage(const string * message) {
	return this->sendMessage(message->c_str());
}
bool ClientConnection::sendMessage(const string message) {
	return this->sendMessage(message.c_str());
}

bool ClientConnection::receiveMessage(char ** message) {
	/*
	 * 1. Receive length of message
	 */
	int intLength;
	char charMessageLength[11];
	// citim lungimea mesajului
	bzero(charMessageLength, 11);
	if(recv(this->socketDescriptor, &charMessageLength, sizeof(charMessageLength), 0) < 0)
		return false;
	intLength = charToInt(charMessageLength);
	/*
	 * 2. Receive actual message
	 */
	char * recvMessage;

	// alocam memorie
	recvMessage = new char[intLength];
	bzero(recvMessage, intLength);

	// primim mesajul
	if(recv(this->socketDescriptor, recvMessage, intLength, 0) < 0)
		return false;

	// alocam memorie pentru pointer-ul de return si copiem continutul in el.
	// V -- this method does not work ; posibila rezolvare message[length] = '\0'
//	*message = new char[intLength];
//	bzero(*message, intLength);
//	strncpy(*message, recvMessage, intLength);
	// V -- instead i use this method WARNING there may be a memory leak because of this
	recvMessage[intLength] = '\0'; // -> asta rezolva problema mesajelor pe care strncpy le produce in client
	*message = recvMessage;

	// aici la copiere face o chestie ciudata,
	// probabil ar trebui sa puna un caracter de final deci sa se aloce cu un char mai mult si sa fie de final ala.. cred

	// dealocam memoria
	// delete(recvMessage);

	return true;
}
//bool ClientConnection::receiveMessage(char * message) {
//	return this->receiveMessage(&message);
//}
//bool ClientConnection::receiveMessage(string message) {
//	char * recvMessage = NULL;
//	this->receiveMessage(&recvMessage);
//	cout << recvMessage << endl;
//	//*message = (new string());
//	message = recvMessage;
//	cout << message << endl;
//	return true;
//}
//bool ClientConnection::receiveMessage(string message) {
//	this->receiveMessage(&message);
//	return true;
//}

int ClientConnection::charToInt(char * c) {
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

void ClientConnection::closeConnection() {
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "closing socket ", this->socketDescriptor);
	close((long)this->socketDescriptor);
}
