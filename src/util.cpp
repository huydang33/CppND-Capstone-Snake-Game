#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "util.h"

void setEchoMode(bool enable) 
{
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	if (enable) {
	term.c_lflag |= ECHO;  // Turn off display characters
	} else {
	term.c_lflag &= ~ECHO;  // Turn on display characters
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void hidePassword(std::string &password)
{
	password = "";
	std::string pwd;
	// Turn off display characters
	setEchoMode(false);
	std::cout << "Enter password: ";
	std::cin >> pwd;

	for(int i = 0; i < pwd.length(); i++)
	{
		char ch = pwd[i];
		if (ch == '\n' || ch == '\r') 
		{
			break;
		}
		if (ch == 127 && password.length() > 0) {
			password.pop_back();
			std::cout << "\b \b";
		} else {
			password.push_back(ch);
			std::cout << "*";
		}
	}
    std::cout << std::endl;
	// Turn on display characters
	setEchoMode(true);
}