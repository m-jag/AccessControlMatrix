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
	// Add the existing objects into the new subject
	map<string, vector<int>> objs;
	for (auto& obj : objects)
	{
		objs.emplace(make_pair(obj, vector<int>()));
	}

	// Add the new subject to the matrix
	matrix.emplace(make_pair(sbj_name, objs));
}
void AccessControlMatrix::removeSubject(string sbj_name)
{
	matrix.erase(sbj_name);
}

void AccessControlMatrix::addObject(string obj_name)
{
	// Add the new object to every subject
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
	// Check if right is already given
	bool newRight = true;
	for (auto r : matrix.at(subject).at(object))
	{
		newRight = newRight && (right != r);
	}

	if (newRight)
	{
		matrix.at(subject).at(object).push_back(right);
		cout << "Right Set! (" << subject << ", " << object << ", " << right << ")" << endl;
	}
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
		for (auto obj : objects)
		{
			// Project widths of row values
			int projected_width = 0;
			int num_rights = subj.second.at(obj).size();
			switch (num_rights)
			{
				case 0: 0; break;
				case 1: projected_width = 1; break; //#
				case 2: projected_width = 5; break; //# & #
				default: projected_width = (int)num_rights * 3; //#, #, #, & #	
			}
			column_widths[col_pos] = max(column_widths[col_pos], projected_width);
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
		for (auto obj : objects)
		{
			cout << " | ";
			string rights_formatted = "";
			int pos = 0;
			vector<int> rights = subj.second.at(obj);
			for (auto right: rights)
			{ 
				if (rights_formatted != "")
				{
					if (rights.size() != 2)
					{
						rights_formatted += ",";
					}
					rights_formatted += " ";

					if (pos == rights.size() - 1)
					{
						rights_formatted += "& ";
					}
				}
				rights_formatted += to_string(right);
				pos++;
			}
			cout << setw(column_widths[col_pos]) << rights_formatted;
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
	rights.push_back("read");
	rights.push_back("write");
	AccessControlMatrix matrix = AccessControlMatrix(rights);
	
	matrix.addSubject("courtney");
	matrix.addSubject("ben");
	matrix.addSubject("john");
	
	matrix.addObject("john");
	matrix.addObject("discord");
	matrix.addObject("outlook");
	
	matrix.setRight("admin", "discord", 0);
	matrix.setRight("admin", "discord", 1);
	matrix.setRight("admin", "discord", 2);
	matrix.setRight("admin", "discord", 3);


	matrix.setRight("admin", "john", 0);
	matrix.setRight("admin", "john", 1);
	matrix.setRight("admin", "john", 2);

	matrix.setRight("admin", "admin", 0);
	matrix.setRight("admin", "admin", 1);
	
	matrix.setRight("ben", "discord", 0);
	matrix.setRight("ben", "discord", 2);
	matrix.setRight("ben", "discord", 3);

	matrix.setRight("john", "outlook", 0);
	
	cout << endl;
	matrix.printMatrix();
}