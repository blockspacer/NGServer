#include "DatabaseInstance.h"

#include "./App/ConnectData.h"

MysqlConnection DBInstance::conn = MysqlConnection(MYSQL_SERVER,
                                                   MYSQL_USER,
                                                   MYSQL_PASSWORD,
                                                   MYSQL_DBNAME);

MysqlConnection DBInstance::GetConnection() {
    return conn;
}

MysqlQuery DBInstance::GetQuery() {
    return query_;
}

MysqlResult DBInstance::GetResult() {
    return result_;
}