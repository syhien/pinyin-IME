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
void word2pinyin(wstring word, int cur_position, string pinyin_string);

map<string, vector<wchar_t>> pinyin;
map<string, vector<wstring>> pinyin_to_words;
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
		cout << "输入拼音\n";
		string pinxie;
		vector<string> yinjie;
		wstring words;
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
			cout << "输入了错误的拼音\n";
			while (!_kbhit());
			continue;
		}
		string yinjie_string = "";
		for (auto i = yinjie.begin(); i != yinjie.end(); i++)
			if (i != yinjie.end() - 1)
				yinjie_string += *i + "'";
			else
				yinjie_string += *i;
		cout << yinjie_string << endl;
		while (!_kbhit());
		continue;
	}
	return 0;
}

void pinyin2data()
{
	cout << "初始化拼音中\n";
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

string wstring2string(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

void dictionary_input(wifstream &fin)
{
	cout << "初始化词典中\n";
	setlocale(LC_ALL, "chs");
	fin.imbue(locale("chs"));
	wstring pinxie_string;
	int words_number;
	wstring word;
	int word_count;
	while (fin >> pinxie_string >> words_number)
		for (int i = 0; i < words_number; i++)
			fin >> word >> word_count, pinyin_to_words[wstring2string(pinxie_string)].push_back(word), dictionary[word] = word_count;
	fin.close();
	fin.clear();
}