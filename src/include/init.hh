#ifndef __INIT_HH__
#define __INIT_HH__
#include <map>
#include <string>
#include "redirect.hh"

extern std::map<std::string, RedirectInfo> config;

void load_configuration();

void create_default_file();

#endif