#include "DatabaseInstance.h"

#include "./App/ConnectData.h"

MysqlConnection DBInstance::conn = MysqlConnection(MYSQL_SERVER,
                                                   MYSQL_USER,
                                                   MYSQL_PASSWORD,
                                                   MYSQL_DBNAME);

