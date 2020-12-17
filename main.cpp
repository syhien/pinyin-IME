#include <iostream>
#include <fstream>
using namespace std;
void txt2data();

int main()
{
	ifstream fin;
	fin.open("IME.data", ios::in);
	if (!fin)
		txt2data();
	return 0;
}