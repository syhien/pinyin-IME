# pinyin-IME 自带词库、支持用户词库的拼音输入法

## 需求分析



## 项目难点

1. 即使使用wchar_t处理汉字，1个汉字还是会被分成2个位置存储，没法进行处理

先设置好locale("chs")在中国，剩下的交给visual studio和c++就可以了