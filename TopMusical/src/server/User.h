/*
 * User.h
 *
 *  Created on: Dec 30, 2015
 *      Author: francisc
 */

#ifndef USER_H_
#define USER_H_


#include "iostream"
#include "string"
#include "vector"

using namespace std;

static const int USER_TYPE_ADMINISTRATOR = 1;
static const int USER_TYPE_NORMAL		 = 0;

static const int RESTRICT_TYPE_VOTE    = 1;
static const int RESTRICT_TYPE_COMMENT = 2;

class User {

	private:
		int id;
		const string * name;
		const string * email;
		int userType;
		int restricType;
	public:

		User(int, const string *, const string *, int, int);
//		~User();

		const char * getEmail();
		const char * getName();
		int getId();
		char * getCharId();
		bool isUserAdministrator();
		bool isVoteRestricted();
		bool isCommentRestricted();
};

#endif /* USER_H_ */
