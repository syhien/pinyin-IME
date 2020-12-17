#include <iostream>
#include <fstream>
#include <map>
#include <locale>
#include <vector>
#include <ctime>
#include <wchar.h>
#include <wctype.h>
using namespace std;
void pinyin2data();
void txt2data();
void dictionary_input(wifstream& fin);

map<string, vector<wchar_t>> pinyin;
map<wstring, long long> dictionary;

int main()
{
	pinyin2data();
	wifstream fin("dictionary.data", ios::in);
	if (!fin)
		txt2data();
	else
		dictionary_input(fin);
	return 0;
}

void pinyin2data()
{
	setlocale(LC_ALL, "chs");
	wifstream fin;
	fin.open("pinyin.txt", ios::in);
	fin.imbue(locale("chs"));
	wstring line;
	while (fin >> line)
	{
		string en = "";
		int i;
		for (i = 0; i < line.length() and (line[i] >= 'a' and line[i] <= 'z'); i++)
			en += line[i];
		for (i++; i < line.length(); i++)
			pinyin[en].push_back(line[i]);
		wstring tmp = L" ";
		for (auto& i : pinyin[en])
		{
			tmp[0] = i;
			dictionary[tmp] = 0;
		}
	}
	fin.close();
	fin.clear();
}

void dictionary_input(wifstream &fin)
{
	setlocale(LC_ALL, "chs");
	fin.imbue(locale("chs"));
	wstring word;
	int word_count;
	while (fin >> word >> word_count)
		dictionary[word] = word_count;
	fin.close();
	fin.clear();
}