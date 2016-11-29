/*
 * DatabaseConnection.h
 *
 *  Created on: Dec 27, 2015
 *      Author: francisc
 */

#ifndef DATABASECONNECTION_H_
#define DATABASECONNECTION_H_

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "iostream"
#include "string"

#include "Logger.h"
#include "User.h"

using namespace std;

class DatabaseConnection {

	private:
		sql::Driver     * driver;
		sql::Connection * connection;
		sql::Statement  * statement;
		sql::ResultSet  * resultSet;

		char * hostname = "tcp://127.0.0.1:3306";
		char * user     = "root";
		char * password = "a3bcFiGO";
		char * schema   = "topMusical";

		string errorWhat;
		string errorTime;
		string mysqlErrorCode;
		string sqlState;

		Logger * logger;

	public:
		DatabaseConnection(Logger *);
		 ~DatabaseConnection();

		bool     registerUser(char *, char *, char *);
		bool     isUserRegistered(char *, char *);
		User   * getUserInstance(char *, char *);
		string * getNormalUserList();
		bool     emailExists(char *);
		bool     insertUserRestricVoteRight(char *);
		bool	 updateUserMakeAdministrator(char*);

		string * getGeneralTop();
		string * getTopByGender(char *);

		bool     addSong(char *, char *, char *, char *);
		bool     songExists(char *);
		bool     deleteSong(char *);
		string * getSongList();
		string * getSongById(char *);

		bool insertVote(char *, char *);
		bool voteExists(char *, char *);

		bool     insertComment(char *, char *, char *);
		string * getCommentBySongId(char *);

		bool genderExists(char *);

		int getLastInsertedRecordId();

		bool connect();
		void closeConnection();

};
#endif /* DATABASECONNECTION_H_ */
