#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "user.h"

void registerNewUser(UserManager &manager)
{
  std::string username, password;
  std::cout << "Enter username: ";
  std::cin >> username;
  std::cout << "Enter password: ";
  std::cin >> password;

  if (manager.addUser(username, password)) {
      std::cout << "User added successfully!" << std::endl;
  } else {
      std::cout << "User already exists!" << std::endl;
  }
}

bool loginUser(UserManager &manager)
{
  std::string username, password;
  std::cout << "Enter username to login: ";
  std::cin >> username;
  std::cout << "Enter password to login: ";
  std::cin >> password;
  bool ret = true;

  if (manager.authenticateUser(username, password)) {
      std::cout << "Login successful!" << std::endl;
      ret = true;
  } else {
      std::cout << "Invalid username or password!" << std::endl;
      ret = false;
  }

  return ret;
}

int main() {
  UserManager manager("/mnt/d/Udacity/C++/CppND-Capstone-Snake-Game/data/users.txt");
  std::string username, password;
  int choice = 0;
  std::string input;
  std::cout << "Register new user press 1, Login press 0 (default value is 0)\n";
  std::getline(std::cin, input);
  if (!input.empty()) {
      std::stringstream(input) >> choice;  // Convert string to int
  }
  if(choice == 0)
  {
    bool login_ret = loginUser(manager);
    // Login fail
    if(login_ret == false)
      return -1;
  }
  else if (choice == 1)
  {
    registerNewUser(manager);
  }
  else
  {
    std::cout << "Invalid choice: " << choice << std::endl;
    return -1;
  }

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
  return 0;
}