#ifndef ACCESSCONTROLMATRIX_H
#define ACCESSCONTROLMATRIX_H

#include "ACMObjects.h"
#include <vector>

/*
		 Obj Obj Obj Obj Obj
		┌───┬───┬───┬───┬───┐
	Sbj |Own|Crt|   |Wrt| Rd|
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
		std::vector<Subject> subjects;
		std::vector<Object> objects;
		std::vector<Rights> rights;

		void addSubject();
		void removeSubject();

		void addObject();
		void removeObject();



	private:
		std::vector<std::vector<std::vector<int>>> matrix;
};
#endif