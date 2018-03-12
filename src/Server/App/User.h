#ifndef NGSERVER_USERAUTH_H
#define NGSERVER_USERAUTH_H


#include <string>

#include "../DatabaseInstance.h"
#include "../Global.h"

class User {
public:
    User() = delete;

    User(const std::string &&name,
         const std::string &&password);

    User(const std::string &&name,
         const std::string &&password,
         const std::string &&email,
         const std::string &&phone);

    ~User() = default;

    /**
     * @brief Auth user return user id,
     *        if success return user id, others return 0
     */
    int Auth();

    bool SignUpCheck();

    void SetName(std::string &text);

    void SetPassword(std::string &text);

    void SetEmail(std::string &text);

    void SetPhone(std::string &text);

    bool IsAuth() const;

    int Id() const;

    std::string Name() const;

private:
    bool is_auth_;
    int id_;
    std::string name_;
    std::string password_;
    std::string email_;
    std::string phone_;
};


#endif //NGSERVER_USERAUTH_H
