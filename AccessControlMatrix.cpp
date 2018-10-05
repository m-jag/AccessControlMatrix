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
		subj.second.emplace(make_pair(obj_name, vector<int>()));
	}
}
void AccessControlMatrix::removeObject(string obj_name)
{
	for (auto& subj : matrix)
	{
		subj.second.erase(obj_name);
	}
}

vector<string> AccessControlMatrix::getAllRights()
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

	// Print the table
	cout << underline_start << setw(column_widths[0]) << "" << end_text;
	col_pos = 1;
	int color_num = 0;
	for (auto obj : objects)
	{
		cout << underline_start << " | ";
		cout << left << setw(column_widths[col_pos]) << obj ;
		cout << end_text;
		col_pos++;
		color_num = (color_num + 1) % 5;
	}
	cout << setfill(' ');
	cout << endl;
	for (auto subj : matrix)
	{
		cout << setw(column_widths[0]) << left << subj.first;
		col_pos = 1;
		for (auto obj : subj.second)
		{
			cout << " | ";
			cout << setw(column_widths[col_pos]) << "";
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