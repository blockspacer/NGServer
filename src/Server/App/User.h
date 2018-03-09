#ifndef NGSERVER_USERAUTH_H
#define NGSERVER_USERAUTH_H


#include <string>

#include "../DatabaseInstance.h"
#include "../Global.h"

class User {
public:
    User() = delete;

    User(const std::string &&name,
         const std::string &&password)
            : is_auth_(false),
              id_(-1),
              name_(name),
              password_(password) {};

    User(const std::string &&name,
         const std::string &&password,
         const std::string &&email,
         const std::string &&phone)
            : is_auth_(false),
              id_(-1),
              name_(name),
              password_(password),
              email_(email),
              phone_(phone) {};

    ~User() = default;

    /**
     * @brief Auth user return user id,
     *        if success return user id, others return 0
     */
    int Auth() {
        auto db = DBInstance::instance();
        db.GetQuery().Execute("use ngserver");
        std::stringstream ss;
        ss << "select id from user where ";
        ss << "name = '" << name_ << "' and ";
        ss << "password = '" << password_ << "'";
        ss << ";";
        auto result = MysqlResult();
        db.GetQuery().ExecuteQuery(result, ss.str());
        if (result.Next()) {
            int id = result.GetInt("id");
            if (id > 0) {
                is_auth_ = true;
                id_ = id;
                return id;
            } else {
                return 0;
            }
        }
        return 0;
    }

    bool SignUpCheck() {
        bool ok = true;
        if (name_.empty()) { return false; }
        else if (password_.empty()) { return false; }
        else if (email_.empty()) { return false; }
        else if (phone_.empty()) { return false; }

        auto db = DBInstance::instance();
        db.GetQuery().Execute("use ngserver");
        std::stringstream ss;
        ss << "select id,name from user where ";
        ss << "name = '" << name_ << "'";
        ss << ";";

        auto result = MysqlResult();
        db.GetQuery().ExecuteQuery(result, ss.str());
        if (result.Next()) {
            int id = result.GetInt("id");
            if (id > 0) {
                DLOG(INFO) << "duplicate name";
                return false;
            }
        }

        return true;
    }

    void SetName(std::string &text) {
        name_ = text;
    }

    void SetPassword(std::string &text) {
        password_ = text;
    }

    void SetEmail(std::string &text) {
        email_ = text;
    }

    void SetPhone(std::string &text) {
        phone_ = text;
    }

    /**
     * @brief Get user id, if success return user id, others return -1
     */
    bool IsAuth() const {
        return is_auth_;
    }

    /**
     * @brief Get user id
     */
    int Id() const {
        return id_;
    }

    /**
     * @brief Get user name
     */
    std::string Name() const {
        return name_;
    }

private:
    bool is_auth_;
    int id_;
    std::string name_;
    std::string password_;
    std::string email_;
    std::string phone_;
};


#endif //NGSERVER_USERAUTH_H
