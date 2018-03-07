link_directories(/usr/lib/)
link_directories(/usr/local/lib/glog/lib)

add_executable(Database
        main.cpp
        MysqlConnection.cpp
        MysqlConnection.h
        RedisConnection.cpp
        RedisConnection.h )

target_link_libraries(Database
        mysqlcppconn
        glog)

include_directories(/usr/local/lib/glog/include)