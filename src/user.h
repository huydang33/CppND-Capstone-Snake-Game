#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <vector>
#include "util.h"

class User {
public:
    // Default constructor
    User(std::string username = "", std::string password = "", std::string score = "");

    // Rule of 5
    ~User() = default; // Destructor

    User(const User& other) = default; // Copy constructor

    User(User&& other) noexcept = default; // Move constructor

    User& operator=(const User& other) = default; // Copy assignment operator

    User& operator=(User&& other) noexcept = default; // Move assignment operator

    // Setters
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setUserscore(const std::string& userscore);

    // Getters
    const std::string& getUsername() const;
    const std::string& getUserscore() const;

    bool checkPassword(const std::string& password) const;

    // Convert User to string representation
    std::string toString() const;

    static User fromString(const std::string& str);

private:
    std::string _username;
    std::string _password;
    std::string _userscore;

    static std::string hashPassword(const std::string& password);
};

class UserManager {
public:
    explicit UserManager(const std::string& filename)
        : _filename(filename) {}

    // Rule of 5
    ~UserManager() = default; // Destructor

    UserManager(const UserManager& other) = default; // Copy constructor

    UserManager(UserManager&& other) noexcept = default; // Move constructor

    UserManager& operator=(const UserManager& other) = default; // Copy assignment operator

    UserManager& operator=(UserManager&& other) noexcept = default; // Move assignment operator

    e_return_result addUser(const std::string& username, const std::string& password);
    e_return_result authenticateUser(const std::string& username, const std::string& password);
    e_return_result loginUser(User &user);
    e_return_result registerNewUser(User &user);
    e_return_result setUser(User &user, std::string const &username, std::string const &password);
    e_return_result loadUsersFromFile();
    e_return_result saveToFile(const std::string username, const int score);

private:
    std::vector<User> _users;
    std::string _filename;

    e_return_result saveUsersToFile(User const user);
};

#endif