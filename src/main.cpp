#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "user.h"
#include "util.h"

int main() 
{
	e_return_result ret = RET_OK;
	UserManager manager(INPUT_TXT_STRING);
	int choice = 0;
	std::string input;
	std::string username = "";
	int user_score = 0;
	User user;

	std::cout << "Register new user press 1, Login press 0 (default value is 0)\n";
	std::getline(std::cin, input);
	if (!input.empty()) {
		std::stringstream(input) >> choice;  // Convert string to int
	}
	if(choice == 0)
	{
		ret = manager.loginUser(user);
	}
	else if (choice == 1)
	{
		ret = manager.registerNewUser(user);
	}
	else
	{
		std::cout << "Invalid choice: " << choice << std::endl;
		ret = RET_NG;
	}

	if(ret == RET_OK)
	{
		constexpr std::size_t kFramesPerSecond{60};
		constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
		constexpr std::size_t kScreenWidth{640};
		constexpr std::size_t kScreenHeight{640};
		constexpr std::size_t kGridWidth{32};
		constexpr std::size_t kGridHeight{32};

		Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
		Controller controller;
		Game game(kGridWidth, kGridHeight);
		game.Run(controller, renderer, kMsPerFrame);
		std::cout << "Game has terminated successfully!\n";
		std::cout << "Score: " << game.GetScore() << "\n";
		std::cout << "Size: " << game.GetSize() << "\n";

		/* If personal score higher than stored user score, then update user score*/
		if(std::stoi(user.getUserscore()) < game.GetScore())
			ret = manager.saveToFile(user.getUsername(), game.GetScore());
	}
	
	return ret;
}