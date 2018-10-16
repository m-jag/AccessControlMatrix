#include "AccessControlMatrix.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Constructor
AccessControlMatrix::AccessControlMatrix(vector<string> rights)
{
	for (int r_val = 0; r_val < rights.size(); r_val++)
	{
		cout << r_val << " " << rights.at(r_val) << endl;
		this->rights.emplace(make_pair(rights.at(r_val), r_val));
	}
	addSubject("admin");
	addSubject("admin");
	for (auto o: objects)
	{
		for (auto r: rights)
		{
			grantRight("admin", o, r);
		}
	}
	setSubject("admin");
}

void AccessControlMatrix::addSubject(string sbj_name)
{
	subjects.push_back(sbj_name);

	// Add the existing objects into the new subject
	map<string, vector<int>> objs;
	for (auto& obj : objects)
	{
		objs.emplace(make_pair(obj, vector<int>()));
	}

	// Add the new subject to the matrix
	matrix.emplace(make_pair(sbj_name, objs));
	addObject(sbj_name);
}
void AccessControlMatrix::removeSubject(string sbj_name)
{
	for (auto subj = subjects.begin(); subj != subjects.end(); ++subj)
	{
		if ((*subj)==sbj_name)
		{
			cout << "Removing " << *subj << endl;
			subjects.erase(subj);
		}
	}
	matrix.erase(sbj_name);
}
void AccessControlMatrix::setSubject(string sbj_name)
{
	accessingSubject = sbj_name;
	cout << "Current User : " << accessingSubject << endl;
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
	objects.erase(remove(objects.begin(), objects.end(), obj_name), objects.end());
	
	for (auto& subj : matrix)
	{
		subj.second.erase(obj_name);
	}

}

vector<string> AccessControlMatrix::getAllRights()
{
	vector<string> list_rights;
	for (auto right: rights)
	{
		list_rights.push_back(right.first);
	}
	return list_rights;
}
void AccessControlMatrix::grantRight(string subject, string object, string right)
{
	try
	{
		int right_val = rights.at(right);

		// Check if right is already given
		bool newRight = true;
		for (auto r : matrix.at(subject).at(object))
		{
			newRight = newRight && (right_val != r);
		}

		if (newRight)
		{
			matrix.at(subject).at(object).push_back(right_val);
			//cout << "Right Set! (" << subject << ", " << object << ", " << right << ")" << endl;
		}
	}
	catch (const out_of_range& oor)
	{
		cout << "No such right \"" + right + "\"" << endl;
	}	
}
int AccessControlMatrix::deleteRight(string object, string subject, string right)
{
	try
	{
		int right_val = rights.at(right);

		// find right position
		bool found = false;
		int rightPos = -1;
		for (auto r : matrix.at(subject).at(object))
		{
			rightPos++;
			if (right_val == r)
			{
				found = true;
				break;
			}
		}

		if (rightPos >= 0 && found)
		{
			matrix.at(subject).at(object).erase(matrix.at(subject).at(object).begin()+rightPos);
			//cout << "Right Removed! (" << subject << ", " << object << ", " << right << ")" << endl;
		}
	}
	catch (const out_of_range& oor)
	{
		cout << "No such right \"" + right + "\"" << endl;
	}
}
vector<string> AccessControlMatrix::getRights(string object, string subject)
{
	vector<string> list_rights;
	for (auto right_num: matrix.at(subject).at(object))
	{
		for (auto right_val: rights)
		{
			if (right_val.second == right_num)
			{
				list_rights.push_back(right_val.first);
				break;
			}
		}
	}
	return list_rights;
}
void transferRights(string subject1, string subject2)
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