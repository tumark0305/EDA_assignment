#include "os_info.h"

os_info::os_info() {
    char buffer[1024];
#ifdef _WIN32
    DWORD length = GetCurrentDirectoryA(sizeof(buffer), buffer);
    if (length == 0) {
        std::cerr << "GetCurrentDirectory() failed with error: " << GetLastError() << std::endl;
    }
#else
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        perror("getcwd() error");
    }
#endif
    path = buffer;
}