# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* libssl-dev
  * sudo apt install libssl-dev

## Basic Build Instructions

1. Clone this repo.
2. Create data user directory which hold users information: `mkdir data && cd data && vi users.txt && chmod 777 users.txt && cd ..`
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## Features add

* Enter player names and password: Password will be encrypted and store in data/users.txt alongs with user name and highest score.
* Difficulty setting: Easy/Normal/Hard
* Reading and Writing user's high score.
* Obstacles in Normal/Hard level: The snake will die if hit the obstacles.
* Poison Food available in Hard Level: There is 30% chance that poisoned the snake and make it go in opposite direction. For example, pressing Up key will result to Down key (effective in 5 second).
* Console output score.

## Addressed rubric points
### Loops, Functions, I/O - meet at lease 2 criteria
* Rubic Point 1/4: The project demonstrates an understanding C++ functions and control structures
Game.cpp line 82: PlaceFood() meet this criteria.

* Rubic Point 2/4: The project accepts user input and processes the input.
Snake.cpp line 5: SetSpeed() meet this criteria.

### Object Oriented Programming - meet at least 3 criteria
* Rubic Point 1/4: One or more classes are added to the project with appropriate access specifiers for class members.
user.cpp : User() meets this criteria when creating a class for User/UserManager class to store/process user information

* Rubic Point 2/4: Class constructors utilize member initialization lists.
user.cpp : User() meets this criteria.

* Rubic Point 1/4: The project uses multithreading.
Renderer.cpp line 91: Render() meets this criteria.

### Memory Management - meet at least 3 criteria
* Rubic Point 1/6: The project makes use of references in function declarations.


* Rubic Point 3/6: The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.


* Rubic Point 6/6: The project uses smart pointers instead of raw pointers.
main.cpp meets this criteria: Use smart pointer to proccess for UserManager class.

### Concurrency - meet at least 2 criteria
* Rubic Point 1/4: The project uses multithreading.
Renderer.cpp line 91: Render() meets this criteria.

* Rubic Point 2/4: A promise and future is used in the project.
Renderer.cpp line 71: Render() meets this criteria.

## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
