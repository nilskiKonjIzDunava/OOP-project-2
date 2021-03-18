#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>
#include<iostream>

using namespace std;

class NegativeNumberOfElementsException :public exception
{
public:
	NegativeNumberOfElementsException(const char* msg) : exception(msg) {}
};


////////////////////////////////////////////////////////////////////////////////////////////////

class IncorrectNumberOfPortsException :public exception // ovo ako se pokusa da se stavi npr nesto na treci ulaz elementa, a on ima dva
{
public:
	IncorrectNumberOfPortsException(const char* msg) : exception(msg) {}

};
class NegativePortNumberException :public IncorrectNumberOfPortsException
{
public:
	NegativePortNumberException(const char* msg) :IncorrectNumberOfPortsException(msg) {}
};

class NotConnectedPorts :public IncorrectNumberOfPortsException
{
public:
	NotConnectedPorts(const char* msg) :IncorrectNumberOfPortsException(msg) {}
};

class AlreadyConnectedPortException :public IncorrectNumberOfPortsException
{
public:
	AlreadyConnectedPortException(const char* msg) :IncorrectNumberOfPortsException(msg) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////

class NotUniqueIdException :public exception
{
public:
	NotUniqueIdException(const char* msg) : exception(msg) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////

class NegativeTimeException :public exception
{
public:
	NegativeTimeException(const char* msg) :exception(msg) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////

class NegativeFrequencyException :public exception
{
public:
	NegativeFrequencyException(const char* msg) :exception(msg) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////

class ZeroFrequencyException :public exception
{
public:
	ZeroFrequencyException(const char* msg) :exception(msg) {}
};
#endif // !_EXCEPTIONS_H_


