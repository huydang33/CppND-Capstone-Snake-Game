#include "user.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include "util.h"

// Constructor
User::User(std::string username, std::string password, std::string userscore)
    : _username(std::move(username)), _password(hashPassword(password)), _userscore(std::move(userscore)) {}

// Setter
void User::setUsername(const std::string &username)   { this->_username = username; };
void User::setPassword(const std::string &password)   { this->_password = password; };
void User::setUserscore(const std::string &userscore) { this->_userscore = userscore; };

// Getter
const std::string& User::getUsername() const {
    return _username;
}

const std::string& User::getUserscore() const {
    return _userscore;
}

bool User::checkPassword(const std::string& password) const {
    return _password == hashPassword(password);
}

// Convert to string to store in txt file
std::string User::toString() const {
    return _username + " " + _password + " " + _userscore;
}

// Create instance User from string
User User::fromString(const std::string& str) {
    std::string init_score = "0";
    std::istringstream iss(str);
    std::string username, hashedPassword;
    iss >> username >> hashedPassword >> init_score;
    User user(username, "", init_score);
    user._password = hashedPassword; // Store hash password
    return user;
}

std::string User::hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return oss.str();
}

e_return_result UserManager::loadUsersFromFile() {
    e_return_result ret = RET_OK;
    std::ifstream file(_filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << _filename << " for writing!" << std::endl;
        ret = RET_NG_SYS;
    }
    else {
        std::string line;
        while (std::getline(file, line)) {
            _users.push_back(User::fromString(line));
        }
    }

    return ret;
}

e_return_result UserManager::saveUsersToFile(User const user) {
    e_return_result ret = RET_OK;
    std::ofstream file(_filename, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing!" << std::endl;
        ret = RET_NG_SYS;
    }

    if (ret == RET_OK)
        file << user.toString() << std::endl;

    return ret;
}

e_return_result UserManager::addUser(const std::string& username, const std::string& password) {
    e_return_result ret = RET_OK;
    // Check if user exists
    for (const auto& user : _users) {
        if (user.getUsername() == username) {
            ret = RET_NG_USER;
        }
    }

    if(ret == RET_OK)
    {
        // Add new user
        User new_user = User(username, password, "0");
        _users.push_back(new_user);
        ret = saveUsersToFile(new_user);
    }
    return ret;
}

e_return_result UserManager::authenticateUser(const std::string& username, const std::string& password) {
    for (const auto& user : _users) {
        if (user.getUsername() == username && user.checkPassword(password)) {
            return RET_OK; // login success
        }
    }
    return RET_NG_USER; // login failed
}

e_return_result UserManager::setUser(User &user, std::string const &username, std::string const &password)
{
    e_return_result ret = RET_OK;
    // Assign username and password to the argument user
    user.setUsername(username);
    user.setPassword(password);
    for (const auto& loop_user : _users) {
        if (loop_user.getUsername() == user.getUsername())
            user.setUserscore(loop_user.getUserscore());
    }

    return ret;
}

e_return_result UserManager::registerNewUser(User &user)
{
    e_return_result ret = RET_OK;
	std::string username, password;
	std::cout << "Enter username: ";
	std::cin >> username;
	hidePassword(password);

    ret = addUser(username, password);
	if (ret == RET_OK) {
	    std::cout << "User added successfully!" << std::endl;
        ret = setUser(user, username, password);
	} else if (ret == RET_NG_USER) {
	    std::cerr << "Error in adding new user!" << std::endl;
	} else {
        /* */
    }

    return ret;
}

e_return_result UserManager::loginUser(User &user)
{
	std::string username, password;
	std::cout << "Enter username to login: ";
	std::cin >> username;
	hidePassword(password);
	e_return_result ret = RET_OK;

	if (authenticateUser(username, password) == RET_OK) 
	{
		std::cout << "Login successful!" << std::endl;
        ret = setUser(user, username, password);
	} 
	else 
	{
		std::cout << "Invalid username or password!" << std::endl;
		ret = RET_NG_USER;
	}

	return ret;
}

e_return_result UserManager::saveToFile(const std::string username, const int value)
{
    e_return_result ret = RET_OK;
    std::ifstream fileIn(_filename);
    std::vector<std::string> lines;
    bool userFound = false;

    // Read line by line and check if user exists
    if (fileIn.is_open()) {
        std::string line;
        while (std::getline(fileIn, line)) 
        {
            std::istringstream iss(line);
            std::string fileUsername;
            int current_value;
            std::string password;

            // Read username, password, and current_value
            if (iss >> fileUsername >> password >> current_value) {
                if (fileUsername == username) {
                    // Replace
                    line = fileUsername + " " + password + " " + std::to_string(value);
                    userFound = true;
                }
            }
            lines.push_back(line);
        }
        fileIn.close();
    }
    else
    {
        std::cerr << "Cannot open: " << _filename << std::endl;
        ret = RET_NG_SYS;
    }

    // Return NG if not found username
    if (!userFound) {
        ret = RET_NG_SYS;
    }

    if (ret == RET_OK)
    {
        // Store lines back to file
        std::ofstream fileOut(_filename);
        if (fileOut.is_open()) {
            for (const auto& line : lines) 
            {
                fileOut << line << std::endl;
            }
            fileOut.close();
        } else {
            std::cerr << "Cannot open: " << _filename << std::endl;
            ret = RET_NG_SYS;
        }
    }

    return ret;
}