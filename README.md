# pinyin-IME 自带词库、支持用户词库的拼音输入法

## 需求分析



## 项目难点

1. 即使使用wchar_t处理汉字，1个汉字还是会被分成2个位置存储，没法进行处理

先设置好locale("chs")在中国，剩下的交给visual studio和c++就可以了

2. 难以分辨串中某个字符是不是中文

先设置locale,`setlocale(LC_ALL, "chs")`设为大陆简体中文。然后直接调用`iswupper()` `iswlower()` `iswdigit()` `iswpunct()` `iswspace()`判断是否是大小写字母、数字、标点、空格，再找该字符是否存在至少一个对应的拼音（筛去繁体字）。

