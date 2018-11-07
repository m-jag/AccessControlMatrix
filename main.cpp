#include "AccessControlMatrix.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<string> rights = vector<string> ();
	AccessControlMatrix matrix = AccessControlMatrix(rights);

	bool george = matrix.addSubject("admin", "george", "pass");
	cout << "Command " << (george? "Accepted": "Failed") << " : " << "Create subject george" << endl;
	bool discord = matrix.addObject("george", "discord");
	cout << "Command " << (discord? "Accepted": "Failed") << " : " << "Create object discord" << endl;
	matrix.printMatrix();
}