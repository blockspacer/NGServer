#include "User.h"

User::User(const std::string &&name,
     const std::string &&password)
        : is_auth_(false),
          id_(-1),
          name_(name),
          password_(password) {};


User::User(const std::string &&name,
     const std::string &&password,
     const std::string &&email,
     const std::string &&phone)
        : is_auth_(false),
          id_(-1),
          name_(name),
          password_(password),
          email_(email),
          phone_(phone) {};

int User::Auth() {
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

bool User::SignUpCheck() {
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

void User::SetName(std::string &text) {
    name_ = text;
}

void User::SetPassword(std::string &text) {
    password_ = text;
}

void User::SetEmail(std::string &text) {
    email_ = text;
}

void User::SetPhone(std::string &text) {
    phone_ = text;
}

bool User::IsAuth() const {
    return is_auth_;
}

int User::Id() const {
    return id_;
}

std::string User::Name() const {
    return name_;
}