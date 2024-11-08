#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <vector>

class User {
public:
    User(std::string username = "", std::string password = "");
    
    const std::string& getUsername() const;
    bool checkPassword(const std::string& password) const;

    // "username password"
    std::string toString() const;

    static User fromString(const std::string& str);

private:
    std::string _username;
    std::string _password;

    // Băm mật khẩu bằng SHA-256
    static std::string hashPassword(const std::string& password);
};

class UserManager 
{
public:
    UserManager(const std::string& filename);

    bool addUser(const std::string& username, const std::string& password);
    bool authenticateUser(const std::string& username, const std::string& password);

private:
    std::vector<User> _users;
    std::string _filename;

    void loadUsersFromFile();
    void saveUsersToFile();
};

#endif