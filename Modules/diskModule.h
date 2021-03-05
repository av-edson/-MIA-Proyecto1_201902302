#ifndef _DISK_MODULE
#define _DISK_MODULE

#include <string>
#include <iostream>
#include <string.h>

bool mkdiskF(int size, std::string fit, std::string units, std::string path);

bool rmdiskF(std::string path);

#endif