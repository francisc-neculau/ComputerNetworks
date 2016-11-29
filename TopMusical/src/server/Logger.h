/*
 * Logger.h
 *
 *  Created on: Dec 28, 2015
 *      Author: francisc
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "iostream"
#include "string"
#include "vector"
#include "cppconn/exception.h"

using namespace std;

#define logInfo( loggerInstance, message ) loggerInstance->output( loggerInstance->logINFO, __LINE__, (char *)__FUNCTION__, __FILE__, message )

class Logger {

	private:

	public:
		static const int logINFO = 1;
		static const int logERROR = 2;

		Logger();
//		~Logger();

		void outputSQL(sql::SQLException &, int, char *, char *);
		void output(int, int, char *, char *, char *, int);
		void output(int, int, char *, char *, char *);
		void output(int, int, char *, char *, string);
		void outputInfo(int, char *, char *, vector<char *> &);
		/*string */ void getTime();

};

#endif /* LOGGER_H_ */
