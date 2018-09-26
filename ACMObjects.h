#ifndef ACMOBJECTS_H
#define ACMOBJECTS_H

#include <string>

class Subject
{
	public:
		std::string getName();
		int getId();

	private:
		std::string name;
		int id;
};

class Object
{
	public:
		std::string getName();
		int getId();

	private:
		std::string name;
		int id;		
};

class Rights
{
	public:
		std::string getName();
		int getId();

	private:
		std::string name;
		int id;		
};
#endif