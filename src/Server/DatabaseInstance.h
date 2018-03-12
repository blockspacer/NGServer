#ifndef NGSERVER_DATABASEINSTANCE_H
#define NGSERVER_DATABASEINSTANCE_H

#include "../Database/MysqlConnection.h"

using namespace Database;

class DBInstance {
public:
    static DBInstance &instance() {
        static auto *instance = new DBInstance();
        return *instance;
    }

    static MysqlConnection conn;

    MysqlConnection GetConnection();

    MysqlQuery GetQuery();

    MysqlResult GetResult();

private:
    DBInstance() = default;

    MysqlQuery query_ = MysqlQuery(conn);

    MysqlResult result_ = {};

};


#endif //NGSERVER_DATABASEINSTANCE_H
