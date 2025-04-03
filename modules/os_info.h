#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#else
#include <unistd.h>
#endif

#ifndef OS_INFO_H
#define OS_INFO_H
#include <iostream>
class os_info {
private:

public:
    std::string path;
    os_info();
};

#endif