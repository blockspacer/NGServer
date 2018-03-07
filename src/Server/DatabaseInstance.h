#ifndef MASSGATE_MOD_DATABASEINSTANCE_H
#define MASSGATE_MOD_DATABASEINSTANCE_H

#include "../Database/MysqlConnection.h"

using namespace Database;

class DBInstance {
public:
    static DBInstance &instance() {
        static auto *instance = new DBInstance();
        return *instance;
    }

    static MysqlConnection conn;

    MysqlConnection GetConnection() {
        return conn;
    }

    MysqlQuery GetQuery() {
        return query_;
    }

    MysqlResult GetResult() {
        return result_;
    }

private:
    DBInstance() = default;

    MysqlQuery query_ = MysqlQuery(conn);

    MysqlResult result_ = {};

};


#endif //MASSGATE_MOD_DATABASEINSTANCE_H
