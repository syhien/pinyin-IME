#include <iostream>
#include <fstream>
#include <map>
#include <locale>
#include <vector>
#include <ctime>
#include <wchar.h>
#include <wctype.h>
#include <Windows.h>
#include <conio.h>
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
	while (1)
	{
		system("cls");
		cout << "ÊäÈëÆ´Òô\n";
		string pinxie;
		vector<string> yinjie;
		bool error_input = 0;
		cin >> pinxie;
		for (int i = 0; i < pinxie.length();)
		{
			if (pinxie[i] == '\'')
			{
				i++;
				continue;
			}
			auto dot_find = pinxie.substr(i).find("'");
			int end_position = dot_find == pinxie.substr(i).npos ? pinxie.length() - 1 : dot_find + i - 1;
			while (end_position >= i)
				if (pinyin.find(pinxie.substr(i, end_position - i + 1)) == pinyin.end())
					end_position--;
				else
					break;
			if (end_position < i)
			{
				error_input = 1;
				break;
			}
			yinjie.push_back(pinxie.substr(i, end_position - i + 1));
			i = end_position + 1;
		}
		if (error_input)
		{
			cout << "ÊäÈëÁË´íÎóµÄÆ´Òô\n";
			while (!_kbhit())
				;
			continue;
		}
		for (auto i = yinjie.begin(); i != yinjie.end(); i++)
			if (i != yinjie.end() - 1)
				cout << *i << "'";
			else
				cout << *i << endl;
		while (!_kbhit())
			;
		continue;
	}
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