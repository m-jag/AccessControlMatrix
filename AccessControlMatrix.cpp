#include "AccessControlMatrix.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Constructor
AccessControlMatrix::AccessControlMatrix(vector<string> rights)
{
	// Add basic rights
	rights.push_back("own");
	string underline_start = "\e[4m";
	string end_text = "\e[0m";
	cout << underline_start << "Rights" << end_text << endl;
	for (int r_val = 0; r_val < rights.size(); r_val++)
	{
		cout << r_val << " " << rights.at(r_val) << endl;
		this->rights.emplace(make_pair(rights.at(r_val), r_val));
	}
	addSubject("admin");
	for (auto s: subjects)
	{
		for (auto r: rights)
		{
			grantRight("admin", s, r);
		}
	}
	for (auto o: objects)
	{
		for (auto r: rights)
		{
			grantRight("admin", o, r);
		}
	}
	setSubject("admin");
}

void AccessControlMatrix::setSubject(string sbj_name)
{
	accessingSubject = sbj_name;
	cout << "Current User : " << accessingSubject << endl;
}

bool AccessControlMatrix::checkRight(string object, string right)
{
	bool hasRight = false;
	int r_val = rights.at(right);
	for (auto r: matrix.at(accessingSubject).at(object))
	{
		hasRight = hasRight || r == r_val;
	}
	return hasRight;
}

bool AccessControlMatrix::subjectExists(string subj_name)
{
	// Check if object exists
	bool exists = false;
	for (auto sbj: subjects)
	{
		if (sbj == subj_name)
		{
			exists = true;
			break;
		}
	}
	return exists;
}

bool AccessControlMatrix::objectExists(string obj_name)
{
	// Check if object exists
	bool exists = false;
	for (auto obj: objects)
	{
		if (obj == obj_name)
		{
			exists = true;
			break;
		}
	}
	return exists;
}

bool AccessControlMatrix::rightExists(string right_name)
{
	// Check if object exists
	bool exists = false;
	for (auto r: rights)
	{
		if (r.first == right_name)
		{
			exists = true;
			break;
		}
	}
	return exists;
}

// R1
// Transfers rights from S1 -> S2
void AccessControlMatrix::transferRights(string subject1, string subject2)
{
	// Check rights


	// Executing commands
	bool subj1Exists = subjectExists(subject1);
	bool subj2Exists = subjectExists(subject2);
	if (subj1Exists && subj2Exists)
	{
		map<string, vector<int>> s1Rights = matrix.at(subject1);
		matrix.at(subject2).swap(s1Rights);
	}
	else
	{
		cout << "Failed " << subject1 << (subj1Exists? " Exists ": " Doesn't Exist ") << subject2 << (subj1Exists? " Exists ": " Doesn't Exist ") << endl;
	}
}

// R2
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

// R3
int AccessControlMatrix::deleteRight(string subject, string object, string right)
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

// R4
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

// R5
void AccessControlMatrix::addObject(string obj_name, bool isSubject)
{
	// Check if object already added
	bool exists = false;
	for (auto obj: objects)
	{
		exists |= obj == obj_name;
	}
	// 

	if (exists) return;
	// Add the new object to every subject
	if (!isSubject) objects.push_back(obj_name);
	for (auto& subj : matrix)
	{
		subj.second.emplace(make_pair(obj_name, vector<int>()));
	}
}

// R6
void AccessControlMatrix::removeObject(string obj_name, bool isSubject)
{
	// Check if object exists
	bool exists = isSubject? subjectExists(obj_name): objectExists(obj_name);

	if (!exists)
	{
		cout << "-No such object \"" + obj_name + "\"" << endl;
		return;
	}

	try
	{
		// Remove object from list of objects
		if (!isSubject)
		{
			//cout << "Removing " << obj_name << endl;
			objects.erase(remove(objects.begin(), objects.end(), obj_name), objects.end());
		}
		
		// Remove object from the ACM
		for (auto& subj : matrix)
		{
			subj.second.erase(obj_name);
		}
	}
	catch (const out_of_range& oor)
	{
		cout << "No such object \"" + obj_name + "\"" << endl;
	}
	
	

}

// R7
void AccessControlMatrix::addSubject(string sbj_name)
{
	subjects.push_back(sbj_name);

	// Add the existing objects into the new subject
	map<string, vector<int>> objs;
	for (auto& sbj : subjects)
	{
		objs.emplace(make_pair(sbj, vector<int>()));
	}
	for (auto& obj : objects)
	{
		objs.emplace(make_pair(obj, vector<int>()));
	}

	// Add the new subject to the matrix
	matrix.emplace(make_pair(sbj_name, objs));
	addObject(sbj_name, true);
}

// R8
void AccessControlMatrix::removeSubject(string sbj_name)
{
	// Check if object exists
	bool exists = subjectExists(sbj_name);

	if (!exists)
	{
		cout << "No such subject \"" + sbj_name + "\"" << endl;
		return;
	}

	try
	{
		removeObject(sbj_name, true);
		for (auto subj = subjects.begin(); subj != subjects.end(); ++subj)
		{
			if ((*subj)==sbj_name)
			{
				//cout << "Removing " << *subj << endl;
				subjects.erase(subj);
			}
		}
		matrix.erase(sbj_name);
	}
	catch (const out_of_range& oor)
	{
		cout << "No such subject \"" + sbj_name + "\"" << endl;
	}
}

void AccessControlMatrix::printMatrix()
{
	string underline_start = "\e[4m";
	string end_text = "\e[0m";

	// Calculate the column widths
	int column_widths[matrix.size() + objects.size() + 1];
	int col_pos = 1; // skip first row since it will be the row names
	// Check column names for width
	for (auto subj: subjects) // Subjects will be listed first
	{
		column_widths[col_pos] = subj.length();
		col_pos++;
	}
	for (auto obj: objects) // Objects will be listed second
	{
		column_widths[col_pos] = obj.length();
		col_pos++;
	}
	// Check rights widths
	for (auto subj : matrix)
	{
		column_widths[0] = max(column_widths[0], (int)subj.first.length()); // Check row name
		col_pos = 1;
		// iterate through subjects first
		for (auto obj : subjects)
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
		// now iterate through objects
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
	for (auto subj: subjects)
	{
		cout << underline_start << " | ";
		cout << left << setw(column_widths[col_pos]) << subj;
		cout << end_text;
		col_pos++;
	}
	for (auto obj : objects)
	{
		cout << underline_start << " | ";
		cout << left << setw(column_widths[col_pos]) << obj ;
		cout << end_text;
		col_pos++;
	}
	cout << setfill(' ');
	cout << endl;
	for (auto subj : matrix)
	{
		cout << setw(column_widths[0]) << left << subj.first;
		col_pos = 1;
		for (auto subj_obj: subjects)
		{
			cout << " | ";
			string rights_formatted = "";
			int pos = 0;
			vector<int> rights = subj.second.at(subj_obj);
			for (auto right: rights)
			{ 
				// if not first number added
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
		for (auto obj : objects)
		{
			cout << " | ";
			string rights_formatted = "";
			int pos = 0;
			vector<int> rights = subj.second.at(obj);
			for (auto right: rights)
			{ 
				// if not first number added
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