#ifndef _DISK_MODULE
#define _DISK_MODULE

#include <string>
#include <iostream>
#include <string.h>

bool mkdiskF(int size, std::string fit, std::string units, std::string path);

bool rmdiskF(std::string path);

bool fdiskF(int _size, std::string _units, std::string _path, std::string _type, std::string _fit,std::string _name, int _add,std::string  _delete );

#endif