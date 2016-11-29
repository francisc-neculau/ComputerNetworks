/*
 * User.cpp
 *
 *  Created on: Dec 30, 2015
 *      Author: francisc
 */


#include "stdio.h"

#include "User.h"

User::User(int id, const string * name, const string * email, int userType, int restrictType) {
	this->id = id;
	this->name = name;
	this->email = email;
	this->userType = userType;
	this->restricType = restrictType;
}

const char * User::getEmail() {
	return (this->email->c_str());
}
const char * User::getName() {
	return (this->name->c_str());
}
int User::getId() {
	return (this->id);
}
char * User::getCharId() {
	char charId[11];
	snprintf(charId, sizeof(charId), "%d", this->id);
	return charId;
}

bool User::isUserAdministrator() {
	return (this->id == USER_TYPE_ADMINISTRATOR);
}

bool User::isVoteRestricted() {
	return (this->restricType == RESTRICT_TYPE_VOTE);
}
bool User::isCommentRestricted() {
	return (this->restricType == RESTRICT_TYPE_COMMENT);
}
