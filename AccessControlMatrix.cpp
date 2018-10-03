#include "AccessControlMatrix.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

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
void AccessControlMatrix::setRight(string subject, string object, int right)
{

}


void AccessControlMatrix::printMatrix()
{
	string underline_start = "\e[4m";
	string end_text = "\e[0m";

	// Calculate the column widths
	int column_widths[objects.size() + 1];
	int col_pos = 1;
	for (auto obj: objects)
	{
		column_widths[col_pos] = obj.length();
		col_pos++;
	}
	for (auto subj : matrix)
	{
		column_widths[0] = max(column_widths[0], (int)subj.first.length());
		col_pos = 1;
		for (auto obj : subj.second)
		{
			column_widths[col_pos] = max(column_widths[col_pos], (int)obj.second.size() * 3 - 2);
			col_pos++;
		}
	}

	cout << "Widths" << endl;
	for (auto w: column_widths)
	{
		cout << w << " ";
	}
	cout << endl;

	// Print the table
	cout << setw(column_widths[0] + 1) << underline_start << end_text;
	col_pos = 1;
	for (auto obj : objects)
	{
		cout << setw(column_widths[col_pos] + 2) << underline_start << " | " << obj << end_text;
		col_pos++;
	}
	cout << endl;
	for (auto subj : matrix)
	{
		cout << setw(column_widths[0]) << subj.first;
		col_pos = 1;
		for (auto obj : subj.second)
		{
			cout << " | ";
			cout << setw(column_widths[col_pos] + 3);
			col_pos++;
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