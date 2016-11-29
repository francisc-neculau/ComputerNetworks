//============================================================================
// Name        : ChessServer.cpp
// Author      : Neculau Francisc
// Version     : 1.0.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include <pthread.h>

#include "ApplicationSession.h"
#include "ClientConnection.h"
#include "DatabaseConnection.h"
#include "Logger.h"

using namespace std;

static const int PORT = 2728;
static const int SERVER_SIZE = 5;

void * treatClient(void *);

int main() {
	/*
	 * 	Declaram un pointer catre clasa Client +
	 * 	un logger petru server
	 */
	ClientConnection * clientConnection;
	Logger * logger = new Logger();
	/*
	 * Declaram vectorul ce tine evidenta tuturor thread-urilor
	 */
	pthread_t threadIdentifierList;//[SERVER_SIZE];
	/*
	 * Declaram structura @clientSocketAddress
	 */
	struct sockaddr_in clientSocketAddress;
	int clientSocketAddressSize;
	int clientSocketDescriptor;
	/*
	 * Setam socketul @serverSocketDescriptor
	 */
	int serverSocketDescriptor;
	int optval = 1;
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "configuring serverSocketDescriptor");
	if((serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		logger->output(logger->logERROR, __LINE__, (char *)__FUNCTION__, __FILE__, "configuring failed");
	setsockopt(serverSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "configured");
	/*
	 * Setam structura @serverSocketAddress
	 */
	struct sockaddr_in serverSocketAddress;
	bzero(&serverSocketAddress, sizeof(serverSocketAddress));
	serverSocketAddress.sin_family      = AF_INET;
	serverSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSocketAddress.sin_port        = htons(PORT);
	/*
	 * Atasam socketului @serverSocketDescriptor adresa @serverSocketAddress
	 */
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "binding serverSocketDescriptor to serverSocketAddress");
	if(bind(serverSocketDescriptor, (struct sockaddr *)&serverSocketAddress, sizeof(struct sockaddr)) == -1)
		logger->output(logger->logERROR, __LINE__, (char *)__FUNCTION__, __FILE__, "binding failed");
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "binded");
	/*
	 * Setam socketulu @serverSocketDescriptor ca socket pasiv ce asteapta conexiuni ( maxim 5 )
	 */
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "start listening");
	if(listen(serverSocketDescriptor, SERVER_SIZE) == -1)
		logger->output(logger->logERROR, __LINE__, (char *)__FUNCTION__, __FILE__, "listening failed");
	logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "started");

	/*
	 * Tratam clientii
	 */
	while(1) {
		/*
		 * Acceptam clientii initializand @clientSocketDescriptor si @clientSocketAddressSize
		 * ( apelul accept e blocant )s
		 */
		clientSocketAddressSize = sizeof(clientSocketAddress);
		bzero(&clientSocketAddress, sizeof(clientSocketAddress));
		clientSocketDescriptor = accept(serverSocketDescriptor,
				(struct sockaddr *) &clientSocketAddress, (unsigned int *) &clientSocketAddressSize);
		if(clientSocketDescriptor < 0)
			cout<<"eroare";
		logger->output(logger->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, "new connection, socket=", clientSocketDescriptor);
		/*
		 * Lansam un nou thread care sa trateze clientul
		 */
		clientConnection = new ClientConnection(clientSocketAddress, clientSocketDescriptor);
		pthread_create(&threadIdentifierList, NULL, treatClient, (void * ) clientConnection);

	}

	return EXIT_SUCCESS;
}

void * treatClient(void * input) {

	ApplicationSession * session = new ApplicationSession((ClientConnection *)(input));
	/*
	 * 1. Initializam comunicarea cu Baza de Date
	 */
	session->start();
	/*
	 * 2. Autentificam clientul
	 */
	session->authentificateClient();
	/*
	 * 3. Comunicam cu clientul DACA e autentificat
	 */
	if(session->isClientAuthentificated())
		session->treatClient();
	/*
	 * 4. Finalizam comunicarea cu Clientul
	 */
	session->close();
	pthread_exit(NULL);

}
