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

	protected:
		bool checkRight(string subject, string object, string right); // Checks rights for the accessing subject
		
		bool subjectExists(string subj_name);
		bool objectExists(string obj_name);
		bool rightExists(string right_name);

		void transferRights(string subject1, string subject2); //R1
		void grantRight(string granting_subject, string subject, string object, string right); //R2
		void deleteRight(string object, string subject, string right); //R3
		vector<string> getRights(string object, string subject); //R4

		void addObject(string creating_subject, string obj_name, bool isSubject = false); //R5
		void removeObject(string subj_name, string obj_name, bool isSubject = false); //R6

		void addSubject(string creating_subject, string sbj_name, string password); //R7
		void removeSubject(string owning_subject, string sbj_name); //R8

	private:
		bool authenticate(string name, string password);
		string accessingSubject;
		map<string, string> subjects;
		map<string, int> rights;
		vector<string> objects;
		map<string, map<string, vector<int>>> matrix;
};
#endif //ACCESSCONTROLMATRIX_H