#include <iostream>
#include <fstream>
#include <map>
#include <locale>
#include <vector>
using namespace std;
void pinyin2data();
void txt2data();

map<string, vector<wchar_t>> pinyin;

int main()
{
	pinyin2data();
	return 0;
}

void pinyin2data()
{
	wifstream fin;
	fin.imbue(locale("chs"));
	fin.open("pinyin.txt", ios::in);
	wstring line;
	while (fin >> line)
	{
		string en = "";
		int i;
		for (i = 0; i < line.length() and (line[i] >= 'a' and line[i] <= 'z'); i++)
			en += line[i];
		for (i++; i < line.length(); i++)
			pinyin[en].push_back(line[i]);
		//DEBUG
		cout << "×ÖÄ¸´®:" << en << " ";
		wcout.imbue(locale("chs"));
		for (auto& j : pinyin[en])
			wcout << j << " ";
		cout << endl;
	}
}
