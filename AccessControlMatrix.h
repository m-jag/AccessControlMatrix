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
		/**TODO**/ 
		int deleteRight(string object, string subject, string right); //R3
		/**TODO**/ 
		vector<string> getRights(string object, string subject); //R4
		/**TODO**/ 
		void transferRights(string subject1, string subject2, string object); //R1
		void printMatrix();

	protected:
		void grantRight(string object, string subject, string right); //R2

		void addObject(string name); //R5
		void removeObject(string name); //R6

		void addSubject(string name); //R7
		void removeSubject(string name); //R8

		
		

		void setSubject(string subject);

	private:
		vector<string> getAllRights();

		string accessingSubject;
		vector<string> subjects;
		map<string, int> rights;
		vector<string> objects;
		map<string, map<string, vector<int>>> matrix;
};
#endif //ACCESSCONTROLMATRIX_H