/*
 * ApplicationSession.h
 *
 *  Created on: Dec 28, 2015
 *      Author: francisc
 */

#ifndef APPLICATIONSESSION_H_
#define APPLICATIONSESSION_H_

#include "iostream"
#include "string"

#include "DatabaseConnection.h"
#include "ClientConnection.h"
#include "Logger.h"
#include "User.h"

using namespace std;

static const string MESSAGE_WELCOME         = "topMusical \n1. log in \n2. sing in \n3. quit \n";
static const string MESSAGE_WRONG_OPTION    = "wrong option..";
static const string MESSAGE_SUCCESS 		= "success! press enter..";
static const string MESSAGE_FAIL	 		= "fail! press enter..";
static const string MESSAGE_SINGED_IN       = "you are signed in";
static const string MESSAGE_MAIN_MENU_ADMIN = "Main Menu -admin- \n1. Show general top \n2. Show custom top \n3. All songs \n4. Add song \n5. Restrict user vote right\n6. Make admin \n7. quit \n";
static const string MESSAGE_MAIN_MENU       = "Main Menu \n1. Show general top \n2. Show custom top \n3. All songs \n4. Add song \n5. quit \n";
static const string MESSAGE_SONGS_MENU		= " 1. pick song \n 2. go back \n";
static const string MESSAGE_SONG_MENU  	 	= " 1. vote song \n 2. comment \n 3. main menu \n 4. quit \n";
static const string MESSAGE_SONG_MENU_ADMIN = " 1. vote song \n 2. comment \n 3. main menu \n 4. delete song \n5. quit \n";
static const string MESSAGE_QUIT            = "q";
static const string MESSAGE_VOTE_RESTRICT	= "vote right halted! press enter..";

class ApplicationSession {

	private:
		DatabaseConnection * databaseConnection;
		ClientConnection   * clientConnection;
		Logger             * logger;
		User			   * user;
		bool clientAuthentificated;

	public:

		ApplicationSession(ClientConnection *);
		~ApplicationSession();

		void start();
		void authentificateClient();
		bool isClientAuthentificated();
		void treatClient();
		void close();

		void showGeneralTop();
		void showCustomTop();

		void addSong();
		void deleteSong(char *);
		void allSongs();
		void pickSong();
		void commentSong(char *);
		void voteSong(char *);

		void restrictUserVoteRight();
		void makeUserAdministrator();

		bool instantiateUser(char *, char *);

};

#endif /* APPLICATIONSESSION_H_ */
