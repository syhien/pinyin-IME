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
extern map<vector<string>, vector<wstring>> dictionary;
extern map<wstring, long long> frequency;

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

void txt2data()
{
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
			vector<string> pinxie;
			for (auto& i : word)
				pinxie.push_back(hanzi_to_pinyin[i]);
			if (find(dictionary[pinxie].begin(), dictionary[pinxie].end(), word) == dictionary[pinxie].end())
				dictionary[pinxie].push_back(word), frequency[word] = 1;
			else
				frequency[word]++;
		}
	}
	fin.close();
	fin.clear();
	wofstream fout("dictionary.data", ios::out);
	fout.imbue(locale("chs"));
	for (auto& i : dictionary)
	{
		fout << i.first.size() << L" ";
		for (auto& j : i.first)
			fout << string2wstring(j) << L" ";
		fout << i.second.size() << L" ";
		for (auto& j : i.second)
			fout << j << L" " << frequency[j] << L" ";
		fout << endl;
	}
	fout.close();
	fout.clear();
}