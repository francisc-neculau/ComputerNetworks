/*
 * Logger.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: francisc
 */

#include "Logger.h"
#include "string.h"
using namespace std;

Logger::Logger() {

}

void Logger::outputSQL(sql::SQLException &e,  int lineNumber, char * functionName, char * fileName) {
	string result;
	cout << ("[");
	cout << (lineNumber);
	cout << (":");
	cout << (functionName);
	cout << (":");
	cout << (fileName);
	cout << ("] -- ");
	(this->getTime());
	cout << (" -- SQL EXCEPTION -- ");
	cout << ("\n");
	cout << ("#ERR : ");
	cout << (e.what());
	cout << ("\n");
	cout << ("#MySQL error code : ");
	cout << (e.getErrorCode());
	cout << ("\n");
	cout << ("#SQLstate : ");
	cout << (e.getSQLState());
	cout << endl;
}

void Logger::output(int logType, int lineNumber, char * functionName, char * fileName, string message) {
	this->output(logType, lineNumber, functionName, fileName, message.c_str());
}

void Logger::output(int logType, int lineNumber, char * functionName, char * fileName, char * message) {
	string result;
	cout << ("[");
	cout << ((lineNumber));
	cout << (":");
	cout << (functionName);
	cout << (":");
	cout << (fileName);
	cout << ("] -- ");
	(this->getTime());
	if(logType == logINFO) {
		cout << (" -- LOG INFO -- ");
	} else if(logType == logERROR) {
		cout << (" -- LOG ERROR -- ");
	}
	cout << (message);
	cout << endl;
}

void Logger::output(int logType, int lineNumber, char * functionName, char * fileName, char * message, int value) {
	string result;
	cout << ("[");
	cout << lineNumber;
	cout << (":");
	cout << (functionName);
	cout << (":");
	cout << (fileName);
	cout << ("] -- ");
	(this->getTime());
	if(logType == logINFO) {
		cout << (" -- LOG INFO -- ");
	} else if(logType == logERROR) {
		cout << (" -- LOG ERROR -- ");
	}
	cout << (message);
	cout << (value);
	cout << endl;
}

void Logger::outputInfo(int lineNumber, char * functionName, char * fileName, vector<char *> & messageVector) {
	string result;
	cout << ("[");
	cout << lineNumber;
	cout << (":");
	cout << (functionName);
	cout << (":");
	cout << (fileName);
	cout << ("] -- ");
	(this->getTime());
	cout << (" -- LOG INFO -- ");
	for (vector<char *>::iterator it = messageVector.begin(); it != messageVector.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

void Logger::getTime() {
	string result;
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	/*result.append((char *)*/cout << (now->tm_year + 1900);
	cout << ("-");
	cout << ((now->tm_mon + 1));
	cout << ("-");
	cout << (now->tm_mday);
	cout << (" ");
	cout << (now->tm_hour);
	cout << (":");
	cout << (now->tm_min);
	cout << (":");
	cout << (now->tm_sec);
	// return result;
}
