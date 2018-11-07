#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>

using namespace std;

class Subject
{
	public:
		Subject(string name, string password);
		string toString();

	private:
		string name;
		string password;
};
#endif //SUBJECT_H