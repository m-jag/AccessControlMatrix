#include "AccessControlMatrix.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<string> rights = vector<string> ();
	rights.push_back("own");
	rights.push_back("create");
	rights.push_back("read");
	rights.push_back("write");
	AccessControlMatrix matrix = AccessControlMatrix(rights);
	matrix.printMatrix();
	matrix.deleteRight("admin", "admin", "own");
	matrix.printMatrix();
	for (auto val: matrix.getRights("admin", "admin"))
	{
		cout << val << " ";
	}
	cout << endl;
}