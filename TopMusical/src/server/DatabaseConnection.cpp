/*
 * Database.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: francisc
 */

#include "stdio.h"

#include "DatabaseConnection.h"

using namespace std;

DatabaseConnection::DatabaseConnection(Logger * logger) {
	this->logger = logger;
}

DatabaseConnection::~DatabaseConnection() {
	delete this->resultSet;
	delete this->statement;
	delete this->connection;
}

bool DatabaseConnection::registerUser(char * name, char * email, char * password) {
	if(this->emailExists(email))
		return false;
	string * query = new string();
	query->append("INSERT INTO `user` (`id`, `name`, `email`, `password`, `userTypeId`) VALUES (NULL, '");
	query->append(name);query->append("', '");
	query->append(email);query->append("', '");
	query->append(password);query->append("', '0')");
	try {
		statement->execute(*query);
		delete query;
		return true;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	return true;
}

bool DatabaseConnection::isUserRegistered(char * email, char * password) {
	string * query = new string();
	query->append("SELECT id FROM `user` WHERE `email` = '");
	query->append(email);query->append("' AND `password` = '");query->append(password);query->append("'");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	if(resultSet->rowsCount() == 1)
		return true;
	return false;
}

bool DatabaseConnection::emailExists(char * email) {
	string * query = new string();
	query->append("SELECT id FROM `user` WHERE `email` = '");
	query->append(email);query->append("'");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	if(resultSet->rowsCount() == 1)
		return true;
	return false;
}

User * DatabaseConnection::getUserInstance(char * email, char * password) {
	string * query1 = new string();
	User * user = NULL;
	query1->append("SELECT id, name, email, userTypeId FROM `user` WHERE `email` = '");
	query1->append(email);
	query1->append("' AND `password` = '");
	query1->append(password);
	query1->append("'");
	try {
		resultSet = statement->executeQuery(*query1);
		delete query1;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query1;
	}
	int id = -1;
	sql::SQLString stringId;
	sql::SQLString name;
	int userType;
	int restrictType = 0;
	if(resultSet->rowsCount() == 1) {
		resultSet->next();
		id       = resultSet->getInt(1);
		stringId = resultSet->getString(1);
		name     = resultSet->getString(2);
		userType = resultSet->getInt(4);
	}
	if(id == -1)
		return user;
	string * query2 = new string();
	query2->append("SELECT * FROM userRestriction WHERE `idUser` = ");
	query2->append(stringId);
	try {
		resultSet = statement->executeQuery(*query2);
		delete query2;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query2;
	}
	if(resultSet->rowsCount() == 1) {
		resultSet->next();
		restrictType = 1;//resultSet->getInt(1);
	}
	user = new User(id, new string(name.c_str()), new string(email), userType, restrictType);
	return user;
}

string * DatabaseConnection::getGeneralTop() {
	string * query = new string();
	string * result = new string();
	query->append("SELECT song.id, song.name, song.link, song.description, gender.name 'gender', vote.votes 'number of votes'"
			"FROM ("
			"	SELECT * "
			"	FROM song "
			"		LEFT JOIN songGender ON song.id=songGender.idSong "
			"	  ) as song "
			"	LEFT JOIN gender ON song.idGender=gender.id "
			"	LEFT JOIN ( "
			"		SELECT songId, COUNT(userId) as votes "
			"		FROM vote "
			"		GROUP BY songId "
			" 		  ) as vote ON song.id=vote.songId "
			"ORDER BY vote.votes DESC");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
	}
	result->append("id, name, link, description, gender, number of votes \n");

	while (resultSet->next()) {
		result->append(resultSet->getString(1));result->append(" ");
		result->append(resultSet->getString(2));result->append(" ");
		result->append(resultSet->getString(3));result->append(" ");
		result->append(resultSet->getString(4));result->append(" ");
		result->append(resultSet->getString(5));result->append(" ");
		result->append(resultSet->getString(6));result->append(" ");
		result->append("\n");
	}
	return result;
}

string * DatabaseConnection::getTopByGender(char * genderId) {
	string * query = new string();
	string * result = new string();
	query->append("SELECT song.id, song.name, song.link, song.description, gender.name 'gender', vote.votes 'number of votes'"
			"FROM ("
			"	SELECT * "
			"	FROM song "
			"		LEFT JOIN songGender ON song.id=songGender.idSong "
			"	  ) as song "
			"	LEFT JOIN gender ON song.idGender=gender.id "
			"	LEFT JOIN ( "
			"		SELECT songId, COUNT(userId) as votes "
			"		FROM vote "
			"		GROUP BY songId "
			" 		  ) as vote ON song.id=vote.songId "
			"WHERE gender.id=");
	query->append(genderId);
	query->append(" ORDER BY vote.votes DESC");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
	}
	result->append("id, name, link, description, gender, number of votes \n");

	while (resultSet->next()) {
		result->append(resultSet->getString(1));result->append(" ");
		result->append(resultSet->getString(2));result->append(" ");
		result->append(resultSet->getString(3));result->append(" ");
		result->append(resultSet->getString(4));result->append(" ");
		result->append(resultSet->getString(5));result->append(" ");
		result->append(resultSet->getString(6));result->append(" ");
		result->append("\n");
	}
	return result;
}

string * DatabaseConnection::getSongById(char * charSongId){
	string * query = new string();
	string * result = new string();
	query->append("SELECT song.id, song.name, song.link, song.description, gender.name 'gender', vote.votes 'number of votes'"
			"FROM ("
			"	SELECT * "
			"	FROM song "
			"		LEFT JOIN songGender ON song.id=songGender.idSong "
			"	  ) as song "
			"	LEFT JOIN gender ON song.idGender=gender.id "
			"	LEFT JOIN ( "
			"		SELECT songId, COUNT(userId) as votes "
			"		FROM vote "
			"		GROUP BY songId "
			" 		  ) as vote ON song.id=vote.songId "
			"WHERE song.id=");
	query->append(charSongId);
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		return result;
		delete query;
	}
	result->append("id, name, link, description, gender, number of votes \n");
	resultSet->next();

	result->append(resultSet->getString(1));result->append(" ");
	result->append(resultSet->getString(2));result->append(" ");
	result->append(resultSet->getString(3));result->append(" ");
	result->append(resultSet->getString(4));result->append(" ");
	result->append(resultSet->getString(5));result->append(" ");
	result->append(resultSet->getString(6));result->append(" ");
	result->append("\n");

	return result;
}

string * DatabaseConnection::getCommentBySongId(char * charSongId) {
	string * query = new string();
	string * result = new string();
	query->append("SELECT user.name, comment.text, comment.date "
			"FROM comment "
			"	LEFT JOIN user ON user.id=comment.userId "
			"WHERE songId=");
	query->append(charSongId);
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		return result;
		delete query;
	}
	result->append("userName, text, date\n");

	while (resultSet->next()) {
		result->append(resultSet->getString(1));result->append(" ");
		result->append(resultSet->getString(2));result->append(" ");
		result->append(resultSet->getString(3));result->append(" ");
		result->append("\n");
	}

	return result;
}

bool DatabaseConnection::addSong(char * name, char * link, char * description, char * charGenderId) {
	if(!this->genderExists(charGenderId))
		return false;
	string * query1 = new string();
	string * query2 = new string();
	int songId;
	char charSongId[11];
	query1->append("INSERT INTO `song` (`id`, `name`, `link`, `description`) VALUES ");
	query1->append("(NULL, '");
	query1->append(name);
	query1->append("', '");
	query1->append(link);
	query1->append("', '");
	query1->append(description);
	query1->append("')");

	try {
		statement->execute(*query1);
		songId = getLastInsertedRecordId();
		delete query1;
		snprintf(charSongId, sizeof(charSongId), "%d", songId);

		query2->append("INSERT INTO `songGender` (`idSong`, `idGender`) VALUES ");
		query2->append("('");
		query2->append(charSongId);
		query2->append("', '");
		query2->append(charGenderId);
		query2->append("')");
		statement->execute(*query2);
		delete query2;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query1;
		delete query2;
		return false;
	}
	return true;
}

int DatabaseConnection::getLastInsertedRecordId() {
	string * query = new string("select last_insert_id() as id");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	resultSet->next();
	return resultSet->getInt(1);
}

bool DatabaseConnection::genderExists(char * charGenderId) {
	string * query = new string();

	query->append("SELECT id FROM `gender` WHERE `id` = '");
	query->append(charGenderId);
	query->append("'");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	if(resultSet->rowsCount() == 1)
		return true;
	return false;
}

bool DatabaseConnection::insertVote( char * charUserId, char * charSongId) {
	if(this->voteExists(charSongId, charUserId))
		return false;
	string * query = new string();
	query->append("INSERT INTO `vote` (`userId`, `songId`) VALUES ");
	query->append("('");
	query->append(charUserId);
	query->append("', '");
	query->append(charSongId);
	query->append("');");

	try {
		statement->execute(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	return true;
}

bool DatabaseConnection::voteExists(char * charSongId, char * charUserId) {
	string * query = new string();

	query->append("SELECT * FROM `vote` WHERE `userId` = '");
	query->append(charUserId);
	query->append("' AND `songId` = '");
	query->append(charSongId);
	query->append("'");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	if(resultSet->rowsCount() >= 1)
		return true;
	return false;
}

bool DatabaseConnection::insertComment(char * charUserId, char * charSongId, char * comment) {
	if(!this->songExists(charSongId))
		return false;
	string * query = new string();
	query->append("INSERT INTO `comment` (`userId`, `songId`, `text`, `date`) VALUES ");
	query->append("('");
	query->append(charUserId);
	query->append("', '");
	query->append(charSongId);
	query->append("', '");
	query->append(comment);
	query->append("', 'current_timestamp()');");

	try {
		statement->execute(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	return true;
}

bool DatabaseConnection::songExists(char * charSongId) {
	string * query = new string();

	query->append("SELECT id FROM `song` WHERE `id` = '");
	query->append(charSongId);
	query->append("'");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	if(resultSet->rowsCount() == 1)
		return true;
	return false;
}

string * DatabaseConnection::getNormalUserList() {
	string * query = new string();
	string * result = new string();
	query->append("SELECT * FROM "
			    "( "
			    "	SELECT user.id, user.name, user.email, userRestriction.idUser "
			    "	FROM user LEFT JOIN userRestriction ON (userRestriction.idUser=user.id) WHERE userTypeId=0 "
			    ") as selection "
				"WHERE idUser is NULL");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		result->append("no users");
		delete query;
		return result;
	}
	result->append("id, name, email \n");
	while(resultSet->next()) {
		result->append(resultSet->getString(1));result->append(" ");
		result->append(resultSet->getString(2));result->append(" ");
		result->append(resultSet->getString(3));result->append(" ");
		result->append("\n");
	}
	return result;
}

bool DatabaseConnection::insertUserRestricVoteRight(char * charUserId) {
//	if(!this->userExists(charSongId))
//		return false;
	string * query = new string();
	query->append("INSERT INTO `userRestriction` (`idUser`) VALUES ('");
	query->append(charUserId);
	query->append("')");
	try {
		statement->execute(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	return true;
}

bool DatabaseConnection::updateUserMakeAdministrator(char * charUserId) {
	string * query = new string();
	query->append("UPDATE `user` SET `userTypeId` = '1' WHERE `user`.`id` = ");
	query->append(charUserId);

	try {
		statement->execute(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
		return false;
	}
	return true;

}

bool DatabaseConnection::deleteSong(char * charSongId) {
	string * query1 = new string();
	string * query2 = new string();
	string * query3 = new string();
	string * query4 = new string();
	query1->append("DELETE FROM `song` WHERE `song`.`id` = ");
	query1->append(charSongId);
	query2->append("DELETE FROM `songGender` WHERE `songGender`.`idSong` = ");
	query2->append(charSongId);
	query3->append("DELETE FROM `comment` WHERE `comment`.`songId` = ");
	query3->append(charSongId);
	query4->append("DELETE FROM `vote` WHERE `vote`.`songId` = ");
	query4->append(charSongId);
	try {
		statement->execute(*query1);
		statement->execute(*query2);
		statement->execute(*query3);
		statement->execute(*query4);
		delete query1;
		delete query2;
		delete query3;
		delete query4;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query1;
		delete query2;
		delete query3;
		delete query4;
		return false;
	}
	return true;
}

string * DatabaseConnection::getSongList() {
	string * query = new string();
	string * result = new string();
	query->append("SELECT song.id, song.name, song.link, song.description FROM song");
	try {
		resultSet = statement->executeQuery(*query);
		delete query;
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		delete query;
	}
	result->append("id, name, link, description \n");

	while (resultSet->next()) {
		result->append(resultSet->getString(1));result->append(" ");
		result->append(resultSet->getString(2));result->append(" ");
		result->append(resultSet->getString(3));result->append(" ");
		result->append(resultSet->getString(4));result->append(" ");
		result->append("\n");
	}
	return result;
}

bool DatabaseConnection::connect() {
	try {
		this->driver     = get_driver_instance();
		this->connection = this->driver->connect(this->hostname, this->user, this->password);
		this->connection->setSchema(this->schema);
		this->statement = this->connection->createStatement();
	} catch (sql::SQLException &e) {
		this->logger->outputSQL(e, __LINE__, (char *)__FUNCTION__, __FILE__);
		return false;
	}
	return true;
}

void DatabaseConnection::closeConnection() {
	this->connection->close();
}
/*
 *
 * Sa se realizeze o aplicatie de tipul client/server pentru managmentul a unui top muzical
 * continand genuri diverse. Aplicatia va avea urmatoarele functionalitati: inregistrarea
 * utilizatorilor de mai multe tipuri: obisnuiti, administrator; logarea in sistem.
 * Comenzile nu vor putea fi executate daca autilizatorul nu este autentificat.
 * Operatii ce trebuie implementate: adaugarea unei melodii la top, votarea unei melodii,
 * afisarea topului general curent in functie de numarul de voturi, afisarea topului pentru
 * un anumit gen de muzica(ex: dance, rock, hip-hop, etc...).
 * Utilizatorii
 * autentificati vor putea posta diferite comentarii asociate unei melodii.
 * Administratorul va putea sterge o melodie din top si va putea restrictiona optiunea de vot a unui utilizator.
 *
 * O melodie va avea un nume,o descriere, [ va apartine unuia sau mai multor genuri de muzica, ]
 *  si va avea asociat un link catre videoclipul sau pe youtube sau alte site-uri asemanatoare.
 *
 *
 */
//int DatabaseConnection::charToInt(char * c) {
//	int result = 0, index = 0, digit = 0;
//
//	if(((int)c[index]) == 45) // negative, skip
//		index++;
//
//	for (; index < strlen(c); index++) {
//		digit = (int)c[index] - 48;
//		if(digit > 9)
//			break; // should log an error here
//		result = result * 10 + digit;
//	}
//
//	if((int)c[0] == 45) // negative, set it
//		result = -result;
//
//	return result;
//}
//try {
//		sql::Driver     * driver;
//		sql::Connection * connection;
//		sql::Statement  * statement;
//		sql::ResultSet  * resultSet;
//
//		/* Create a connection */
//		driver = get_driver_instance();
//		connection = driver->connect("tcp://127.0.0.1:3306", "root", "a3bcFiGO");
//		/* Connect to the MySQL test database */
//		connection->setSchema("test");
//
//		statement = connection->createStatement();
//		resultSet = statement->executeQuery("SELECT 'Hello World!' AS _message");
//		while (resultSet->next()) {
//			cout << "\t... MySQL replies: ";
//			/* Access column data by alias or column name */
//			cout << resultSet->getString("_message") << endl;
//			cout << "\t... MySQL says it again: ";
//			/* Access column fata by numeric offset, 1 is the first column */
//			cout << resultSet->getString(1) << endl;
//		}
//		delete resultSet;
//		delete statement;
//		delete connection;
//
//	} catch (sql::SQLException &e) {
//		cout << "# ERR: SQLException in " << __FILE__;
//		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
//		cout << "# ERR: " << e.what();
//		cout << " (MySQL error code: " << e.getErrorCode();
//		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//	}
