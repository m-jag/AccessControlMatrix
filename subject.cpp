#include "subject.h"


Subject::Subject(string name, string password)
{
	this->name = name;
	this->password = password;
}
string Subject::toString()
{
	return this->name;
}