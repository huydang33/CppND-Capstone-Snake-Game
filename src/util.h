#ifndef UTIL_H
#define UTIL_H

#include <termios.h>
#include <unistd.h>

#define INPUT_TXT_STRING ""

enum e_return_result
{
    RET_OK = 0,
    RET_NG_USER = -1,
    RET_NG_SYS = -2
};

enum GAME_DIFFICULTY_LEVEL {
  DIFF_EASY = 0,
  DIFF_NORMAL = 1,
  DIFF_HARD = 2
};

void setEchoMode(bool enable);
void hidePassword(std::string &password);

#endif