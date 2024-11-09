#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <vector>
#include "util.h"

class User {
public:
    User(std::string username = "", std::string password = "", std::string score = "");
    
    void setUsername(const std::string &username);
    void setPassword(const std::string &password);
    void setUserscore(const std::string &userscore);
    const std::string& getUsername() const;
    const std::string& getUserscore() const;
    bool checkPassword(const std::string& password) const;

    // "username password"
    std::string toString() const;

    static User fromString(const std::string& str);

private:
    std::string _username;
    std::string _password;
    std::string _userscore;

    // Hash password with SHA-256
    static std::string hashPassword(const std::string& password);
};

class UserManager 
{
public:
    UserManager(const std::string& filename);

    e_return_result addUser(const std::string& username, const std::string& password);
    e_return_result authenticateUser(const std::string& username, const std::string& password);
    e_return_result loginUser(User &user);
    e_return_result registerNewUser(User &user);
    e_return_result setUser(User &user, std::string const &username, std::string const &password);
    
    e_return_result saveToFile(const std::string username, const int score);

private:
    std::vector<User> _users;
    std::string _filename;

    void loadUsersFromFile();
    e_return_result saveUsersToFile(User const user);
};

#endif