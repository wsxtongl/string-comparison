#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include<algorithm>
#include <assert.h>
#include<ctime>

#include <cstdlib>
#include <memory>

#if defined(GNUC)

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored “-Wdeprecated-declarations”

#elif defined(_MSC_VER)

#pragma warning(disable : 4996)

#endif




using namespace std;



//动态规划
int min3(int a, int b, int c) {
	a = a < b ? a : b;
	return a < c ? a : c;
}


int LevenshteinDP(wstring str1, wstring str2) {
	
	std::vector<int> v1;
	std::vector<int> v2;
	//交换使str1是短的字符串
	if (str1.length() > str2.length()) {
		std::wstring temp = str1;
		str1 = str2;
		str2 = temp;
	}

	//内外循环次数
	int outTimes = str1.length();
	int innerTimes = str2.length();

	//初始化v1，v2
	for (int i = 0; i <= innerTimes; ++i) {
		v1.push_back(i);
		v2.push_back(0);
	}

	for (int i = 0; i < outTimes; ++i) {
		v2.at(0) = i + 1;
		for (int j = 0; j < innerTimes; ++j) {
			//如果不相等，编辑代价为1
			//找出v1[j+1],v1[j],v2[2]中最小的数，按情况赋给v2[j+1]
			if (str1.at(i) != str2.at(j)) {
				v2.at(j + 1) = min3(v1.at(j + 1), v1.at(j), v2.at(j)) + 1;
			}
			//相等则编辑代价为0
			else if (str1.at(i) == str2.at(j)) {
				v2.at(j + 1) = min3(v1.at(j + 1), v1.at(j), v2.at(j));
			}
		}

		//v1，v2逐次向右平移
		v1.assign(v2.begin(), v2.end());
	}

	//v2的最后一个元素即为距离
	int changTimes = v2.at(innerTimes);
	std::cout << "\nthe distance of two string is " << changTimes;
	return changTimes;
}








std::wstring StringToWstring(const std::string &strInput,unsigned int uCodePage)
{
#ifndef _WIN32
    if (strInput.empty())
    {
        return L"";
    }
    std::string strLocale = setlocale(LC_ALL, "");
    const char* pSrc = strInput.c_str();
    unsigned int iDestSize = mbstowcs(NULL, pSrc, 0) + 1;
    wchar_t* szDest = new wchar_t[iDestSize];
    wmemset(szDest, 0, iDestSize);
    mbstowcs(szDest,pSrc,iDestSize);
    std::wstring wstrResult = szDest;
    delete []szDest;
    setlocale(LC_ALL, strLocale.c_str());
    return wstrResult;
#else
    std::wstring strUnicode = L"";
    if (strInput.length() == 0)
    {
        return strUnicode;
    }
    int iLength = ::MultiByteToWideChar(uCodePage, 0, strInput.c_str(), -1, NULL, 0);
    wchar_t* szDest = new wchar_t[iLength + 1];
    memset(szDest, 0, (iLength + 1) * sizeof(wchar_t));

    ::MultiByteToWideChar(uCodePage, 0, strInput.c_str(), -1, (wchar_t*) szDest, iLength);
    strUnicode = szDest;
    delete[] szDest;
    return strUnicode;
#endif
}

std::string WstringToString(const std::wstring &wstrInput,unsigned int uCodePage)
{
#ifndef _WIN32
    if (wstrInput.empty())
    {
        return "";
    }
    std::string strLocale = setlocale(LC_ALL, "");
    const wchar_t* pSrc = wstrInput.c_str();
    unsigned int iDestSize = wcstombs(NULL, pSrc, 0) + 1;
    char *szDest = new char[iDestSize];
    memset(szDest,0,iDestSize);
    wcstombs(szDest,pSrc,iDestSize);
    std::string strResult = szDest;
    delete []szDest;
    setlocale(LC_ALL, strLocale.c_str());
    return strResult;
#else
    std::string strAnsi = "";
    if (wstrInput.length() == 0)
    {
        return strAnsi;
    }

    int iLength = ::WideCharToMultiByte(uCodePage, 0, wstrInput.c_str(), -1, NULL, 0,NULL, NULL);
    char* szDest = new char[iLength + 1];
    memset((void*) szDest, 0, (iLength + 1) * sizeof(char));

    ::WideCharToMultiByte(uCodePage, 0, wstrInput.c_str(), -1, szDest, iLength, NULL,NULL);

    strAnsi = szDest;

    delete[] szDest;
    return strAnsi;
#endif
}







int main()
{
	
	clock_t start, finish;
	double totaltime;
	
	string str1 = "粤H210D9";
	string str2 = "皖H210D1";
	start = clock();
	unsigned char cha = str1[0];
	int iCode = cha << 8;
	cha = str1[1];
	iCode += cha;
	std::string strCode;
	switch(iCode)   //判断文本前两个字节
	{
	   case 0xfffe:    //65534
	       strCode = "Unicode";
	       break;
	   case 0xfeff:    //65279
	       strCode = "Unicode big endian";
	       break;
	   case 0xefbb:    //61371
	       strCode = "UTF-8";
	       break;
	   default:
	       strCode = "ANSI";
	}

    cout  << strCode << endl;
	std::wstring w_str1 = StringToWstring(str1,0);  //0代表ANSI
    std::wstring w_str2 = StringToWstring(str2,0);
    std::string sub_str1 = WstringToString(w_str1.substr(2),0);
	cout << sub_str1 << " "<< sub_str1.length() << endl;
	for (int i = 0; i < w_str1.size(); i++) 
        {
            if (w_str1[i] > 0x80) {
                cout << i << endl;  //中文位置
            }
        }
    cout << "Dynamic Programming: " << LevenshteinDP(w_str1, w_str2) << endl;
    finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "DP运行时间为" << totaltime << "秒！" << endl;
	return 0;
}
