#include <cstdio>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <glog/logging.h>

int main(int argc, char *argv[]) {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    // connect
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("127.0.0.1:3306", "ubuntu", "123456");

    if (con->isValid()) {
        printf("Connecte to mysql success\n");
    }

    // query
    sql::Statement *stmt;
    stmt = con->createStatement();
    stmt->execute("USE test_db");
    stmt->execute("DROP TABLE IF EXISTS test");
    stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");
    stmt->execute("INSERT INTO test(id, label) VALUES (1, 'a')");

    // fetch result
    sql::ResultSet *res;
    res = stmt->executeQuery("select id, label from test order by id asc;");
    LOG(INFO) << "insert one line";
    while (res->next()) {
        LOG(INFO) << "id = " << res->getInt(1)
                  << " label = " << res->getString("label");
    }

    // prepared statement
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con->prepareStatement(
            "insert into test(id, label) value (?, ?);");

    prep_stmt->setInt(1, 1);
    prep_stmt->setString(2, "a");
    prep_stmt->execute();

    prep_stmt->setInt(1, 2);
    prep_stmt->setString(2, "b");
    prep_stmt->execute();

    LOG(INFO) << "insert two lines";
    res = stmt->executeQuery("select id, label from test order by id asc;");
    while (res->next()) {
        LOG(INFO) << "id = " << res->getInt(1)
                  << " label = " << res->getString("label");
    }

    // delete var
    delete con;
    delete stmt;
    delete prep_stmt;

    return 0;
}