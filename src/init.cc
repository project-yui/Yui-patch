#include "./include/init.hh"
#include "./include/const.hh"
#include <cstring>
#include <Windows.h>
#include <fstream>
#include <ios>
#include <string.h>

void create_default_file()
{
    char	strTmpPath[MAX_PATH];
	GetTempPath(sizeof(strTmpPath), strTmpPath);
    strcat_s(strTmpPath, DEFAULT_INDEX);
    // MessageBoxA(NULL, strTmpPath, NULL, 0);

    std::ofstream fs(strTmpPath, std::ios_base::out);
    const char* index = "require('./launcher.node').load('external_index', module);";
    
    fs.write(index, strlen(index));
    fs.flush();
    fs.close();

}