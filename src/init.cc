#include "./include/init.hh"
#include "./include/const.hh"
#include <cstring>
#include <fstream>
#include <ios>

void create_default_file()
{
    std::ofstream fs(DEFAULT_INDEX, std::ios_base::out);
    const char* index = "require('./launcher.node').load('external_index', module);";
    
    fs.write(index, strlen(index));
    fs.flush();
    fs.close();

}