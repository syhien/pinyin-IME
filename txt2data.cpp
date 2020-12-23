#include <fstream>
#include <map>
#include <vector>
#include <iostream>
#include <locale>
#include <wctype.h>
#include <wchar.h>
#include <algorithm>
#include <Windows.h>
using namespace std;

extern map<string, vector<wchar_t>> pinyin;
extern map<wstring, long long> dictionary;
extern map<string, vector<wstring>> pinyin_to_words;

wstring string2wstring(string str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

void word2pinyin(wstring word, int cur_position, string pinyin_string)
{
	wchar_t c = word[cur_position];
	for (auto& i : pinyin)
		if (find(i.second.begin(), i.second.end(), c) == i.second.end())
			continue;
		else
		{
			if (cur_position == word.length() - 1)
				pinyin_to_words[pinyin_string + i.first].push_back(word);
			else
				word2pinyin(word, cur_position + 1, pinyin_string + i.first + "'");
		}
}

void txt2data()
{
	cout << "初始化语料中\n";
	map <wchar_t, string> hanzi_to_pinyin;
	for (auto& i : pinyin)
		for (auto& j : i.second)
			hanzi_to_pinyin[j] = i.first;
	setlocale(LC_ALL, "chs");
	wifstream fin("data.txt", ios::in);
	fin.imbue(locale("chs"));
	wstring word;
	while (fin >> word)
	{
		bool hanzi = 1;
		bool read_end = 0;
		bool t1, t2, t3, t4, t5;
		for (int i = 0; i < word.length() and hanzi; i++)
			if ((t1 = iswupper(word[i])) or (t4 = iswlower(word[i])) or (t5 = iswdigit(word[i])) or (t2 = iswpunct(word[i])) or (t3 = iswspace(word[i]) or hanzi_to_pinyin.find(word[i]) == hanzi_to_pinyin.end()))
				hanzi = 0;
		if (hanzi)
		{
			if (dictionary.find(word) == dictionary.end())
				dictionary[word] = 0, word2pinyin(word, 0, "");
			else
				dictionary[word]++;
		}
	}
	fin.close();
	fin.clear();
	wofstream fout("dictionary.data", ios::out);
	fout.imbue(locale("chs"));
	for (auto& i : pinyin_to_words)
	{
		fout << string2wstring(i.first) << L"  ";
		fout << i.second.size() << L"  ";
		for (auto& j : i.second)
		{
			fout << j << L" ";
			fout.imbue(locale("C"));
			fout << dictionary[j] << L"  ";
			fout.imbue(locale("chs"));
		}
		fout << endl;
	}
	fout.close();
	fout.clear();
}