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
#include <algorithm>
#include <cmath>
using namespace std;
void pinyin2data();
void txt2data();
void dictionary_input(wifstream& fin);
void word2pinyin(wstring word, int cur_position, string pinyin_string);
void save_dictionary();
wstring string2wstring(string str);

map<string, vector<wchar_t>> pinyin;
map<string, vector<wstring>> pinyin_to_words;
map<wstring, long long> dictionary;

bool words_compare(const wstring& a, const wstring& b)
{
	if (dictionary[a] != dictionary[b])
		return dictionary[a] > dictionary[b];
	return a.length() > b.length();
}

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
		cout << "����ƴ��\n";
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
			cout << "��������ĸ��ƴ\n";
			vector<wstring> words_to_choose;
			for (auto& i : pinyin_to_words)
			{
				vector<int> point_position;
				for (int j = 0; j < i.first.length(); j++)
					if (i.first[j] == '\'')
						point_position.push_back(j);
				if (point_position.size() + 1 != pinxie.length())
					continue;
				if (pinxie[0] != i.first[0])
					continue;
				bool error_jianpin = 0;
				for (int j = 1; j < pinxie.length() and !error_jianpin; j++)
					if (pinxie[j] != i.first[point_position[j - 1] + 1])
						error_jianpin = 1;
				if (error_jianpin)
					continue;
				for (auto& j : i.second)
					words_to_choose.push_back(j);
			}
			sort(words_to_choose.begin(), words_to_choose.end(), words_compare);
			for (auto i = words_to_choose.begin(); i != words_to_choose.end(); i++)
				cout << i - words_to_choose.begin() + 1 << ".", wcout << *i << L"  ";
			cout << endl << "�����ţ�0��ʾ�޺�ѡ�˳����룩��\n";
			int word_choose;
			cin >> word_choose;
			if (!word_choose or word_choose < 0 or word_choose > words_to_choose.size())
			{
				cout << "�˳�����\n";
				cout << "��ESC�˳�����������������������\n";
				if (_getch() == 27)
					break;
				continue;
			}
			cout << "��������";
			wcout << words_to_choose[word_choose - 1] << endl;
			dictionary[words_to_choose[word_choose - 1]]++;
			cout << "��ESC�˳�����������������������\n";
			if (_getch() == 27)
				break;
			continue;
		}
		string yinjie_string = "";
		for (auto i = yinjie.begin(); i != yinjie.end(); i++)
			if (i != yinjie.end() - 1)
				yinjie_string += *i + "'";
			else
				yinjie_string += *i;
		cout << yinjie_string << endl;
		int solved_yin = 0;
		wstring final_word = L"";
		while (solved_yin < yinjie.size())
		{
			string cur_yinjie = yinjie[solved_yin];
			vector<wstring> words_to_choose;
			//����
			for (auto& i : pinyin[cur_yinjie])
			{
				wstring tmp = L"��";
				tmp[0] = i;
				words_to_choose.push_back(tmp);
			}
			//����
			for (int i = 1; solved_yin + i < yinjie.size(); i++)
			{
				cur_yinjie += "'" + yinjie[solved_yin + i];
				if (pinyin_to_words.find(cur_yinjie) != pinyin_to_words.end())
					for (auto& j : pinyin_to_words[cur_yinjie])
						words_to_choose.push_back(j);
			}
			sort(words_to_choose.begin(), words_to_choose.end(), words_compare);
			for (auto i = words_to_choose.begin(); i != words_to_choose.end(); i++)
				cout << i - words_to_choose.begin() + 1 << ".", wcout << *i << L"  ";
			cout << endl << "�����ţ�0��ʾ�޺�ѡ�˳����룩��\n";
			int word_choose;
			cin >> word_choose;
			if (!word_choose or word_choose < 0 or word_choose > words_to_choose.size())
			{
				cout << "�˳�����\n";
				break;
			}
			dictionary[words_to_choose[word_choose - 1]]++;
			final_word += words_to_choose[word_choose - 1];
			solved_yin += words_to_choose[word_choose - 1].length();
		}
		cout << "��������";
		wcout << final_word << endl;
		if (dictionary.find(final_word) == dictionary.end())
			pinyin_to_words[yinjie_string].push_back(final_word), dictionary[final_word] = 393939 + 1;
		cout << "��ESC�˳�����������������������\n";
		if (_getch() == 27)
			break;
	}
	save_dictionary();
	return 0;
}

void save_dictionary()
{
	cout << "���ڱ������ݣ������Ժ��˳�\n";
	wofstream fout;
	fout.open("dictionary.data", ios::out | ios::trunc);
	fout.imbue(locale("chs"));
	setlocale(LC_ALL, "chs");
	for (auto& i : pinyin_to_words)
	{
		fout << string2wstring(i.first) << L"  ";
		fout << i.second.size() << L"  ";
		for (auto& j : i.second)
			fout << j << L" " << dictionary[j] << L"  ";
		fout << endl;
	}
	fout.close();
	fout.clear();
	cout << "���뷨�˳�\n";
}

void pinyin2data()
{
	cout << "��ʼ��ƴ����\n";
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
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

void dictionary_input(wifstream &fin)
{
	cout << "��ʼ���ʵ���\n";
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