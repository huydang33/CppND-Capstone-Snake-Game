#include <iostream>
#include <memory>
#include <string>
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
	std::unique_ptr<UserManager> manager(std::make_unique<UserManager>(std::string(INPUT_TXT_STRING)));
	int choice = 0;
	std::string input;
	std::string username = "";
	int user_score = 0;
	User<std::string, std::string, std::string> user;
	GAME_DIFFICULTY_LEVEL level = DIFF_EASY;

	ret = manager->loadUsersFromFile();
	if(ret == RET_OK)
	{
		std::cout << "Register new user press 1, Login press 0 (default value is 0)\n";
		std::getline(std::cin, input);
		if (!input.empty()) {
			std::stringstream(input) >> choice;  // Convert string to int
		}
		if(choice == 0)
		{
			ret = manager->loginUser(user);
		}
		else if (choice == 1)
		{
			ret = manager->registerNewUser(user);
		}
		else
		{
			std::cout << "Invalid choice: " << choice << std::endl;
			ret = RET_NG_SYS;
		}
		
		if(ret == RET_OK)
		{
			std::string diff_level;

			// Lambda function to validate and assign level
			auto validate_and_assign = [](const std::string& input, GAME_DIFFICULTY_LEVEL& level) {
				// Check if input is a valid integer and within 0, 1, 2
				if (input == "0" || input == "1" || input == "2") {
					level = (GAME_DIFFICULTY_LEVEL)std::stoi(input);
					return true;
				}
				return false;
			};

			std::cout << "Choose difficulty level: Easy/Normal/Hard (0/1/2)\n";
			std::cin >> diff_level;
			if (!validate_and_assign(diff_level, level)) {
				std::cout << "Invalid choice\n";
				ret = RET_NG_USER;
			}
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
			game.Run(controller, renderer, kMsPerFrame, level);
			std::cout << "Game has terminated successfully!\n";
			std::cout << "Score: " << game.GetScore() << "\n";
			std::cout << "Size: " << game.GetSize() << "\n";

			/* If personal score higher than stored user score, then update user score*/
			if(std::stoi(user.getUserscore()) < game.GetScore())
				ret = manager->saveToFile(user.getUsername(), game.GetScore());
		}
	}
	
	return ret;
}