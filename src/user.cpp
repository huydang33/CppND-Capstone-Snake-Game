#include "user.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>

// Constructor
User::User(std::string username, std::string password)
    : _username(std::move(username)), _password(hashPassword(password)) {}

// Getter
const std::string& User::getUsername() const {
    return _username;
}

bool User::checkPassword(const std::string& password) const {
    return _password == hashPassword(password);
}

// Convert to string to store in txt file
std::string User::toString() const {
    return _username + " " + _password;
}

// Create instance User from string
User User::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string username, hashedPassword;
    iss >> username >> hashedPassword;
    User user(username, "");
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

// UserManager class implementation
UserManager::UserManager(const std::string& filename) : _filename(filename) {
    loadUsersFromFile();
}

void UserManager::loadUsersFromFile() {
    std::ifstream file(_filename);
    std::string line;
    while (std::getline(file, line)) {
        _users.push_back(User::fromString(line));
    }
}

void UserManager::saveUsersToFile() {
    // std::ofstream file(_filename);
    std::ofstream file(_filename, std::ios::out | std::ios::app); // Thêm quyền ghi (append)
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing!" << std::endl;
    }

    for (const auto& user : _users) {
        file << user.toString() << std::endl;
    }
}

bool UserManager::addUser(const std::string& username, const std::string& password) {
    // Check if user exists
    for (const auto& user : _users) {
        if (user.getUsername() == username) {
            return false;
        }
    }
    // Add new user
    _users.push_back(User(username, password));
    saveUsersToFile();
    return true;
}

bool UserManager::authenticateUser(const std::string& username, const std::string& password) {
    for (const auto& user : _users) {
        if (user.getUsername() == username && user.checkPassword(password)) {
            return true; // login success
        }
    }
    return false; // login failed
}