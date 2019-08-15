/*
 * Exceptions.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* This file contains custom-defined exceptions thrown to signal errors
   or problems in various parts of the code.
   All of them have a constructor with a string, and the string can
   be retrieved using the getMessage() function (not what()).
   The specific strings that are used to signal particular errors are
   discussed where they are thrown.
*/

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>
#include <string>

/* This exception is thrown when there are problems opening data files,
   or when data files have format errors. */

class FileException : public std::exception {
public:
	FileException (std::string message) { _message = message; }
	~FileException () throw () {}
	std::string getMessage () const { return _message; }
private:
	std::string _message;
};

#endif /* EXCEPTIONS_H_ */
