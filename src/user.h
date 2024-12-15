#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <vector>
#include "util.h"

template <typename UsernameType, typename PasswordType, typename ScoreType>
class User {
public:
    User(UsernameType username = UsernameType(), PasswordType password = PasswordType(), ScoreType score = ScoreType())
        : _username(std::move(username)), _password(hashPassword(password)), _userscore(std::move(score)) {}

    // Rule of 5
    ~User() = default; // Destructor
    User(const User& other) = default; // Copy constructor
    User(User&& other) noexcept = default; // Move constructor
    User& operator=(const User& other) = default; // Copy assignment operator
    User& operator=(User&& other) noexcept = default; // Move assignment operator

    // Setters
    void setUsername(const UsernameType& username);
    void setPassword(const PasswordType& password);
    void setUserscore(const ScoreType& userscore);

    // Getters
    const UsernameType& getUsername() const;
    const ScoreType& getUserscore() const;

    bool checkPassword(const PasswordType& password) const;

    // Convert User to string representation
    std::string toString() const;

    static User fromString(const std::string& str);

private:
    UsernameType _username;
    PasswordType _password;
    ScoreType _userscore;

    static PasswordType hashPassword(const PasswordType& password);
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
    e_return_result loginUser(User<std::string, std::string, std::string> &user);
    e_return_result registerNewUser(User<std::string, std::string, std::string> &user);
    e_return_result setUser(User<std::string, std::string, std::string> &user, std::string const &username, std::string const &password);
    e_return_result loadUsersFromFile();
    e_return_result saveToFile(const std::string username, const int score);

private:
    std::vector<User<std::string, std::string, std::string>> _users;
    std::string _filename;

    e_return_result saveUsersToFile(User<std::string, std::string, std::string> const user);
};

#endif