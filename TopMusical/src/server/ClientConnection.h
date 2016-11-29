/*
 * Client.h
 *
 *  Created on: Dec 27, 2015
 *      Author: francisc
 */

#include "iostream"
#include "string"

#include "Logger.h"

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#ifndef CLIENT_H_
#define CLIENT_H_

class ClientConnection {

	private:
		struct sockaddr_in socketAddress;
		int socketDescriptor;
		Logger * logger;
	public:

		ClientConnection(struct sockaddr_in, int);
		~ClientConnection();

		// Poate ar fi mai interesant o metoda comunicate cu parametri cerere si raspuns
		// Ar fi interesant ca metoda de comunicare sa aiba overloading, o forma a ei fiind
		// connection_status -> default sa fie connected si sa mai fie close connection
		bool sendMessage(const char *);
		bool sendMessage(const string *);
		bool sendMessage(const string);
		bool receiveMessage(char **);
//		bool receiveMessage(char *);
//		bool receiveMessage(string *);
//		bool receiveMessage(string);

		int charToInt(char *);

		int getSocketDescriptor();
		void closeConnection();

};

#endif /* CLIENT_H_ */
