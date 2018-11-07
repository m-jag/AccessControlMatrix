#ifndef ACCESSCONTROLMATRIX_H
#define ACCESSCONTROLMATRIX_H

#include "subject.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

/*
		  ↙ Access Control Matrix
		 Obj Obj Obj Sbj Sbj
		┌───┬───┬───┬───┬───┐
	Sbj |Own|Crt|   |Wrt| Rd|<-- Capabilities of the subject
		├───┼───┼───┼───┼───┤
	Sbj |   |Own|   |   |   |
		├───┼───┼───┼───┼───┤
	Sbj |   |   |   |Wrt|   |
		├───┼───┼───┼───┼───┤
	Sbj |   |   |   |   |   |
		├───┼───┼───┼───┼───┤
	Sbj |   |   |   |   |   |
		└───┴───┴───┴───┴───┘
*/

class AccessControlMatrix
{
	public:
		AccessControlMatrix(vector<string> rights);
		void printMatrix();
		
		bool checkRight(Subject subject, string object, string right); // Checks rights for the accessing subject
		
		bool subjectExists(string subj_name);
		bool objectExists(string obj_name);
		bool rightExists(string right_name);

		bool transferRights(Subject subject1, Subject subject2); //R1
		bool grantRight(Subject granting_subject, Subject subject, string object, string right); //R2
		bool deleteRight(Subject deleting_subject, Subject subject, string object, string right); //R3
		vector<string> getRights(string object, string subject); //R4

		bool addObject(Subject creating_subject, string obj_name, bool isSubject = false); //R5
		bool removeObject(Subject subj_name, string obj_name, bool isSubject = false); //R6

		bool addSubject(Subject creating_subject, string sbj_name, string password); //R7
		bool removeSubject(Subject owning_subject, string sbj_name); //R8

	private:
		bool authenticate(Subject name);
		map<Subject, string> subjects;
		map<string, int> rights;
		vector<string> objects;
		map<string, map<string, vector<int>>> matrix;
};
#endif //ACCESSCONTROLMATRIX_H