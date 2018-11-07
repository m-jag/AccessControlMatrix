#ifndef ACCESSCONTROLMATRIX_H
#define ACCESSCONTROLMATRIX_H

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
		
		bool checkRight(string subject, string object, string right); // Che  cks rights for the accessing subject
		
		bool subjectExists(string subj_name);
		bool objectExists(string obj_name);
		bool rightExists(string right_name);

		bool transferRights(string subject1, string subject2); //R1
		bool grantRight(string granting_subject, string subject, string object, string right); //R2
		bool deleteRight(string object, string subject, string right); //R3
		vector<string> getRights(string object, string subject); //R4

		bool addObject(string creating_subject, string obj_name, bool isSubject = false); //R5
		bool removeObject(string subj_name, string obj_name, bool isSubject = false); //R6

		bool addSubject(string creating_subject, string sbj_name, string password); //R7
		bool removeSubject(string owning_subject, string sbj_name); //R8

	private:
		string authenticatedSubject;
		int timeAuthenticated;
		bool authenticate(string name);
		map<string, string> subjects;
		map<string, int> rights;
		vector<string> objects;
		map<string, map<string, vector<int>>> matrix;
};
#endif //ACCESSCONTROLMATRIX_H