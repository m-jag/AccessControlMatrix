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
		void addSubject(string name);
		void removeSubject(string name);

		void addObject(string name);
		void removeObject(string name);

		vector<string> getAllRights();
		void setRight(string object, string subject, int right);

		void printMatrix();


	private:
		vector<string> rights;
		vector<string> objects;
		map<string, map<string, vector<int>>> matrix;
};
#endif