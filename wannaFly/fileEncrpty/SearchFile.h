//
// Created by 10069 on 2020/12/24.
//

#ifndef WANNAFLY_SEARCHFILE_H
#define WANNAFLY_SEARCHFILE_H


#include "encrptyStd.h"
using namespace std;
class SearchFile {
private:

    list<string> baseFilePath, filepath;
    map<string, bool> fliter;
    void getFilesAllWithFliter(string path, list<string>& files) {
        //文件句柄
        intptr_t hFile = 0;
        //文件信息
        struct _finddata_t fileinfo;
        string p;
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
            do {
                if ((fileinfo.attrib & _A_SUBDIR)) {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                        //files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                        getFilesAllWithFliter(p.assign(path).append("\\").append(fileinfo.name), files);
                    }
                }
                else {
                    string name = fileinfo.name;
                    int last = name.find_last_of(".");
                    if (last != string::npos)
                    {
                        name = name.substr(last + 1);
                        if (fliter.find(name) != fliter.end()) {
                            files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                        }
                    }
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            _findclose(hFile);
        }
    }
public:
    SearchFile(list<string> baseFilePath, list<string> fliter) {
        this->baseFilePath = baseFilePath;
        for (string each : fliter) {
            this->fliter[each] = true;
        }
    }
    void toSearchFilePath(void)
    {

        for (string each : this->baseFilePath)
        {
            this->getFilesAllWithFliter(each, this->filepath);
        }

    }
    list<string> getAllFilePath()
    {
        return this->filepath;
    }

};


#endif //WANNAFLY_SEARCHFILE_H
