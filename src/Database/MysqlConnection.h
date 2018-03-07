#ifndef MASSGATE_MOD_CONNECTION_H
#define MASSGATE_MOD_CONNECTION_H


#include <string>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <glog/logging.h>

namespace Database {

class MysqlConnection {
public:
    explicit MysqlConnection(const std::string &server_name,
                             const std::string &user_name,
                             const std::string &password,
                             const std::string &database_name,
                             bool read_only = true) {
        server_name_ = server_name;
        user_name_ = user_name;
        password_ = password;
        database_name_ = database_name;

        driver_ = nullptr;
        conn_ = nullptr;
    };

    virtual ~MysqlConnection() = default;

    bool IsConnected() {
        if (conn_) {
            return conn_->isValid();
        }
        return false;
    }

    bool Connect() {
        driver_ = sql::mysql::get_mysql_driver_instance();
        conn_ = driver_->connect(server_name_, user_name_, password_);
        return conn_->isValid();
    }

    void DisConnect() {
        conn_->close();
    }

    bool IsReadOnly() {
        return true;
    }

    void Close() {
        conn_->close();
    }

    bool IsClosed() const {
        return conn_->isClosed();
    }

    std::string GetClientInfo() const {
        return conn_->getClientInfo();
    }

    std::string GetSchema() const {
        return conn_->getSchema();
    }

    std::string GetCatalog() const {
        return conn_->getCatalog();
    }

    sql::Connection *GetConnection() const {
        return conn_;
    }

    int GetMajorVersion() const {
        return driver_->getMajorVersion();
    }

    int GetMinorVersion() const {
        return driver_->getMinorVersion();
    }

    int GetPatchVersion() const {
        return driver_->getPatchVersion();
    }

    const std::string &GetName() const {
        return driver_->getName();
    }

private:
    sql::mysql::MySQL_Driver *driver_;
    sql::Connection *conn_;

    std::string server_name_;
    std::string user_name_;
    std::string password_;
    std::string database_name_;

    bool connection_is_read_only_;
};


class MysqlResult {
public:
    MysqlResult() = default;

    ~MysqlResult() = default;

    void Result(sql::ResultSet *re) {
        res_ = re;
        res_->getRow();
    }

    bool Next() {
        return res_->next();
    }

    bool Previous() {
        return res_->previous();
    }

    std::istream *GetBlob(uint32_t columnIndex) const {
        return res_->getBlob(columnIndex);
    }

    std::istream *GetBlob(const std::string &columnLabel) const {
        return res_->getBlob(columnLabel);
    }

    bool GetBoolean(uint32_t columnIndex) const {
        return res_->getBoolean(columnIndex);
    }

    bool GetBoolean(const std::string &columnLabel) const {
        return res_->getBoolean(columnLabel);
    }

    long double GetDouble(uint32_t columnIndex) const {
        return res_->getDouble(columnIndex);
    }

    long double GetDouble(const std::string &columnLabel) const {
        return res_->getDouble(columnLabel);
    }

    int32_t GetInt(uint32_t columnIndex) const {
        return res_->getInt(columnIndex);
    }

    int32_t GetInt(const std::string &columnLabel) const {
        return res_->getInt(columnLabel);
    }

    uint32_t GetUInt(uint32_t columnIndex) const {
        return res_->getUInt(columnIndex);
    }

    uint32_t GetUInt(const std::string &columnLabel) const {
        return res_->getUInt(columnLabel);
    }

    int64_t GetInt64(uint32_t columnIndex) const {
        return res_->getInt64(columnIndex);
    }

    int64_t GetInt64(const std::string &columnLabel) const {
        return res_->getInt64(columnLabel);
    }

    uint64_t GetUInt64(uint32_t columnIndex) const {
        return res_->getUInt(columnIndex);
    }

    uint64_t GetUInt64(const std::string &columnLabel) const {
        return res_->getUInt(columnLabel);
    }

    std::string GetString(uint32_t columnIndex) const {
        return res_->getString(columnIndex);
    }

    std::string GetString(const std::string &columnLabel) const {
        return res_->getString(columnLabel);
    }

    bool IsNull(uint32_t columnIndex) const {
        return res_->isNull(columnIndex);
    }

    bool IsNull(const std::string &columnLabel) const {
        return res_->isNull(columnLabel);
    }

    void AfterLast() {
        res_->afterLast();
    }

    bool IsAfterLast() const {
        return res_->isAfterLast();
    }

    void First() {
        res_->first();
    }

    bool IsFirst() const {
        return res_->isFirst();
    }

    void BeforeFirst() {
        res_->beforeFirst();
    }

    bool IsBeforeFirst() const {
        return res_->isBeforeFirst();
    }

    void Last() {
        res_->last();
    }

    bool IsLast() const {
        return res_->isLast();
    }

    void Close() {
        res_->close();
    }

    bool IsClose() {
        return res_->isClosed();
    }

    size_t RowsCount() const {
        return res_->rowsCount();
    }

    sql::ResultSet *GetResult() const {
        return res_;
    }

    unsigned long long GetAffectedNumberOfRows() const {
        return affected_number_of_rows_;
    }

    unsigned int GetFieldCount() const {
        return field_count_;
    }

private:
    sql::ResultSet *res_;
    unsigned long long affected_number_of_rows_;
    unsigned int field_count_;
};


class MysqlResultMeta {
public:
    MysqlResultMeta() = default;

    ~MysqlResultMeta() = default;

private:

};


class MysqlQuery {
public:
    MysqlQuery(MysqlConnection &connecton)
            : conn_(connecton) {
        stmt_ = conn_.GetConnection()->createStatement();
    }

    ~MysqlQuery() = default;

    bool Select(MysqlResult &result, const std::string &query) {
        return true;
    }

    bool Update(MysqlResult &result, const std::string &query) {
        return true;
    }

    bool Insert(MysqlResult &result, const std::string &query) {
        return true;
    }

    bool Delete(MysqlResult &result, const std::string &query) {
        return true;
    }

    bool Commit() {
        return true;
    }

    bool Execute(const std::string &query) {
        stmt_->execute(query);
        return true;
    }

    bool ExecuteQuery(MysqlResult &result, const std::string &query) {
        auto res = stmt_->executeQuery(query);
        result.Result(res);
        return true;
    }

    void PrepareStatement(const std::string &query) {
        // prep_stmt_ = conn_.GetConnection()->prepareStatement(query);
    }

private:
    MysqlConnection conn_;
    sql::Statement *stmt_;
    // sql::PreparedStatement *prep_stmt_;
};

}


#endif //MASSGATE_MOD_CONNECTION_H
