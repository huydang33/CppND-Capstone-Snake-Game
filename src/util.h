#ifndef UTIL_H
#define UTIL_H

#include <termios.h>
#include <unistd.h>

#define INPUT_TXT_STRING "/mnt/d/Udacity/C++/CppND-Capstone-Snake-Game/data/users.txt"

enum e_return_result
{
    RET_OK = 0,
    RET_NG = -1
};

void setEchoMode(bool enable);
void hidePassword(std::string &password);

#endif