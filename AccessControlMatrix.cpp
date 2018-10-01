#include "AccessControlMatrix.h"
#include <iostream>

using namespace std;

AccessControlMatrix::AccessControlMatrix(vector<string> rights)
{
	addSubject("admin");
	addObject("admin");
}
void AccessControlMatrix::addSubject(string sbj_name)
{
	map<string, vector<int>> objs;
	for (auto& obj : objects)
	{
		objs.emplace(make_pair(obj, vector<int>()));
	}
	matrix.emplace(make_pair(sbj_name, objs));
}
void AccessControlMatrix::removeSubject(string sbj_name)
{
	matrix.erase(sbj_name);
}

void AccessControlMatrix::addObject(string obj_name)
{
	objects.push_back(obj_name);
	for (auto& subj : matrix)
	{
		cout << subj.first << endl;
		subj.second.emplace(make_pair(obj_name, vector<int>()));
		cout << subj.second.size() << endl;
	}
}
void AccessControlMatrix::removeObject(string obj_name)
{
	for (auto& subj : matrix)
	{
		subj.second.erase(obj_name);
	}
}

vector<string> AccessControlMatrix::getRights()
{
	return rights;
}

void AccessControlMatrix::printMatrix()
{
	string underline_start = "\e[4m";
	string end_text = "\e[0m";
	cout << "\t";
	for (auto obj : objects)
	{
		cout << underline_start << " | " << obj << end_text;
	}
	cout << endl;
	for (auto subj : matrix)
	{
		cout << subj.first << "\t";
		for (auto obj : subj.second)
		{
			cout << " | " << "\t";
		}
		cout << endl;
	}
}

int main()
{
	vector<string> rights = vector<string> ();
	rights.push_back("own");
	rights.push_back("create");
	AccessControlMatrix matrix = AccessControlMatrix(rights);
	matrix.addSubject("john");
	matrix.addObject("john");
	matrix.addObject("discord");
	matrix.addObject("outlook");
	cout << endl;
	matrix.printMatrix();
}