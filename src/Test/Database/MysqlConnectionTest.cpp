#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Database/MysqlConnection.h"

using namespace Database;

TEST(MysqlConnection, Temp) {
    auto conn = MysqlConnection("tcp://127.0.0.1:3306", "root", "1234", "mysql");

    conn.Connect();
    ASSERT_TRUE(conn.IsConnected());

    conn.DisConnect();
    ASSERT_FALSE(conn.IsConnected());

    conn.Connect();
    ASSERT_TRUE(conn.IsConnected());

    auto res = MysqlResult();
    auto query = MysqlQuery(conn);

    query.Execute("CREATE DATABASE IF NOT EXISTS test_db;");
    query.Execute("USE test_db");
    query.Execute("DROP TABLE IF EXISTS test");
    query.Execute("CREATE TABLE test(id INT, label CHAR(1))");
    query.Execute("INSERT INTO test(id, label) VALUES (1, 'a')");
    query.Execute("INSERT INTO test(id, label) VALUES (2, 'b')");
    query.Execute("INSERT INTO test(id, label) VALUES (3, 'c')");
    query.Execute("INSERT INTO test(id, label) VALUES (4, 'd')");

    query.ExecuteQuery(res, "select id, label from test order by id asc;");

    std::string comapre[] = {"1 a", "2 b", "3 c", "4 d"};
    int i = 0;
    while (res.Next()) {
        LOG(INFO) << "line " << i + 1;
        ASSERT_EQ(res.GetString("id") +
                  " " +
                  res.GetString("label"),
                  comapre[i]);
        LOG(INFO) << "id = " << res.GetInt(1)
                  << " label = " << res.GetString("label");
        ++i;
//        LOG(INFO) << res.IsAfterLast();
//        LOG(INFO) << res.IsFirst();
//        LOG(INFO) << res.IsBeforeFirst();
    }

    res.BeforeFirst();
    ASSERT_TRUE(res.IsBeforeFirst());

    res.First();
    ASSERT_TRUE(res.IsFirst());

    res.Last();
    ASSERT_TRUE(res.IsLast());

    res.AfterLast();
    ASSERT_TRUE(res.IsAfterLast());

    ASSERT_EQ(res.RowsCount(), 4);

    LOG(INFO) << "Client Info: " << conn.GetClientInfo();
    LOG(INFO) << "Schema: " << conn.GetSchema();
    LOG(INFO) << "Catalog: " << conn.GetCatalog();

    LOG(INFO) << conn.GetName() << " version: "
              << conn.GetMajorVersion() << "."
              << conn.GetMinorVersion() << "."
              << conn.GetPatchVersion();

}