#include "AccessControlMatrix.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Constructor
AccessControlMatrix::AccessControlMatrix(vector<string> rights)
{
	string adminName = "admin";

	// Add basic rights
	rights.push_back("own");
	string underline_start = "\e[4m";
	string end_text = "\e[0m";
	for (int r_val = 0; r_val < rights.size(); r_val++)
	{
		//cout << r_val << " " << rights.at(r_val) << endl;
		this->rights.emplace(make_pair(rights.at(r_val), r_val));
	}
	string password;
	cout << "Enter a password for " << adminName << ": ";
	cin >> password;

	// Add admin
	subjects.emplace(make_pair(adminName, password));

	// Add the existing objects into the new subject
	map<string, vector<int>> objs;
	objs.emplace(make_pair(adminName, vector<int>()));

	// Add the new subject to the matrix
	matrix.emplace(make_pair(adminName, objs));

	// Give ownership of admin to admin
	int right_val = this->rights.at("own");

	matrix.at(adminName).at(adminName).push_back(right_val);
}

bool AccessControlMatrix::checkRight(Subject subject, string object, string right)
{
	// Check values exist
	bool sExists = subjectExists(subject);
	bool oExists = objectExists(object) || subjectExists(object);
	bool rExists = rightExists(right);
	if (!sExists || !oExists || !rExists)
	{
		return false;
	}

	// Check if the subject has the given right for the given object
	bool hasRight = false;
	int r_val = rights.at(right);
	for (auto r: matrix.at(subject).at(object))
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
		if (sbj.first == subj_name)
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
bool AccessControlMatrix::transferRights(Subject subject1, Subject subject2)
{
	bool success = false;
	if (authenticate(subject1))
	{
		// Check rights
		if (checkRight(subject1, subject2, "own"))
		{
			// Executing commands
			bool subj1Exists = subjectExists(subject1);
			bool subj2Exists = subjectExists(subject2);
			if (subj1Exists && subj2Exists)
			{
				map<string, vector<int>> s1Rights = matrix.at(subject1);
				matrix.at(subject2).swap(s1Rights);
				success = true;
			}
			else if (!(subj1Exists && subj2Exists))
			{
				cout << "Failed " << subject1 << (subj1Exists? " Exists ": " Doesn't Exist ") << subject2 << (subj1Exists? " Exists ": " Doesn't Exist ") << endl;
			}
		}
		else
		{
			cout << "Failed " << subject1 << " doesn't own " << subject2 << endl;
		}
	}
	return success;
}

// R2
bool AccessControlMatrix::grantRight(string granting_subject, string subject, string object, string right)
{
	bool success = false;
	try
	{
		if (authenticate(granting_subject))
		{
			// Check rights
			if (checkRight(granting_subject, subject, "own"))
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
					success = true;
					//cout << "Right Set! (" << subject << ", " << object << ", " << right << ")" << endl;
				}
			}
		}
	}
	catch (const out_of_range& oor)
	{
		cout << "No such right \"" + right + "\"" << endl;
	}
	return success;
}

// R3
bool AccessControlMatrix::deleteRight(Subject deleting_subject, Subject subject, string object, string right)
{
	bool success = false;
	try
	{
		if (authenticate(deletingSubject))
		{
			// Check rights
			if (checkRight(subject, object, "own"))
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
					success = true;
					//cout << "Right Removed! (" << subject << ", " << object << ", " << right << ")" << endl;
				}
			}
		}
	}
	catch (const out_of_range& oor)
	{
		cout << "No such right \"" + right + "\"" << endl;
	}
	return success;
}

// R4
vector<string> AccessControlMatrix::getRights(string subject, string object)
{
	vector<string> list_rights;

	if (authenticate(subject))
	{
		// Check rights
		if (checkRight(subject, object, "own"))
		{
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
		}
	}
	return list_rights;
}

// R5
bool AccessControlMatrix::addObject(string creating_subject, string obj_name, bool isSubject)
{
	bool success = false;
	// Check if object already added
	bool exists = false;
	for (auto obj: objects)
	{
		exists |= obj == obj_name;
	}
	// 

	if (!exists)
	{
		if (authenticate(creating_subject))
		{
			success = true;
			// Add the new object to every subject
			if (!isSubject) objects.push_back(obj_name);
			for (auto& subj : matrix)
			{
				subj.second.emplace(make_pair(obj_name, vector<int>()));
			}

			// Grant ownership to all subjects that own the given subject and subjects that own that subject
			vector<string> owners;
			owners.push_back(creating_subject);
			for (auto owner: owners)
			{
				for (auto subject: subjects)
				{
					if (checkRight(subject.first, owner, "own"))
					{
						if (!(find(owners.begin(), owners.end(), subject.first) != owners.end()))
						{
							owners.push_back(subject.first);
						}
					}
				}
			}
			for (auto owner: owners)
			{
				grantRight(owner, owner, obj_name, "own");
			}
		}
	}
	return success;
}

// R6
bool AccessControlMatrix::removeObject(string owning_subject, string obj_name, bool isSubject)
{
	bool success = false;
	// Check if object exists
	bool exists = isSubject? subjectExists(obj_name): objectExists(obj_name);

	if (!exists)
	{
		cout << "No such object \"" + obj_name + "\"" << endl;
	}
	else
	{
		if (authenticate(owning_subject))
		// Check rights
		if (checkRight(owning_subject, obj_name, "own"))
		{
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
				success = true;
			}
			catch (const out_of_range& oor)
			{
				cout << "No such object \"" + obj_name + "\"" << endl;
			}
		}
	}
	return success;
}

// R7
bool AccessControlMatrix::addSubject(string creating_subject, string sbj_name, string password)
{
	bool success = false;
	if (authenticate(creating_subject))
	{
		subjects.emplace(make_pair(sbj_name, password));

		// Add the existing objects into the new subject
		map<string, vector<int>> objs;
		for (auto& sbj : subjects)
		{
			objs.emplace(make_pair(sbj.first, vector<int>()));
		}
		for (auto& obj : objects)
		{
			objs.emplace(make_pair(obj, vector<int>()));
		}

		// Add the new subject to the matrix
		matrix.emplace(make_pair(sbj_name, objs));
		addObject(creating_subject, sbj_name, true);
		// Grant ownership to all subjects that own the given subject and subjects that own that subject
		vector<string> owners;
		owners.push_back(creating_subject);
		for (auto owner: owners)
		{
			for (auto subject: subjects)
			{
				if (checkRight(subject.first, owner, "own"))
				{
					if (!(find(owners.begin(), owners.end(), subject.first) != owners.end()))
					{
						owners.push_back(subject.first);
					}
				}
			}
		}
		for (auto owner: owners)
		{
			grantRight(owner, owner, sbj_name, "own");
		}
		// Grant ownsership to the subject over itself
		grantRight(creating_subject, sbj_name, sbj_name, "own");
		success = true;
	}
	return success;
}

// R8
bool AccessControlMatrix::removeSubject(string owning_subject, string sbj_name)
{
	bool success = false;
	// Check if subject exists
	if (!subjectExists(sbj_name))
	{
		cout << "No such subject \"" + sbj_name + "\"" << endl;
	}
	else
	{
		if (authenticate(owning_subject))
		{

			// Check rights
			if (checkRight(owning_subject, sbj_name, "own"))
			{
				try
				{
					removeObject(owning_subject, sbj_name, true);
					for (auto subj = subjects.begin(); subj != subjects.end(); ++subj)
					{
						if (subj->first==sbj_name)
						{
							//cout << "Removing " << *subj << endl;
							subjects.erase(subj);
						}
						if (subj == subjects.end())
						{
							break;
						}
					}
					matrix.erase(sbj_name);
					success = true;
				}
				catch (const out_of_range& oor)
				{
					cout << "No such subject \"" + sbj_name + "\"" << endl;
				}
			}
			else
			{
				cout << "Ownership failure on removing " << sbj_name << " by " << owning_subject <<  endl;
			}
		}
	}
	return success;
}

bool AccessControlMatrix::authenticate(string name)
{
	bool success = false;
	string password;
	cout << name << endl;
	cout << "Password: ";
	cin >> password;
	if (subjects.at(name) != password)
	{
		cout << "Incorrect Password" << endl;
	}
	else if (subjects.at(name) == password)
	{
		success = true;
	}
	return success;
}

void AccessControlMatrix::printMatrix()
{
	string underline_start = "\e[4m";
	string end_text = "\e[0m";


	cout << underline_start << "Rights" << end_text << endl;
	for (auto right: rights)
	{
		cout << right.first << " " << right.second << endl;
	}

	// Calculate the column widths
	int column_widths[matrix.size() + objects.size() + 1];
	int col_pos = 1; // skip first row since it will be the row names
	// Check column names for width
	for (auto subj: subjects) // Subjects will be listed first
	{
		column_widths[col_pos] = subj.first.length();
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
			int num_rights = subj.second.at(obj.first).size();
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
		cout << left << setw(column_widths[col_pos]) << subj.first;
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
			vector<int> rights = subj.second.at(subj_obj.first);
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