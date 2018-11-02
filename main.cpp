#include "AccessControlMatrix.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<string> rights = vector<string> ();
	AccessControlMatrix matrix = AccessControlMatrix(rights);
	matrix.printMatrix();
}