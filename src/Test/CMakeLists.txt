link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)
link_directories(/usr/local/lib/glog/lib)
link_directories(/usr/local/lib/uvw)
link_directories(/usr/local/lib/uvw)

add_executable(Test
        _Test.cpp
        Util/TimeableTest.cpp Database/MysqlConnectionTest.cpp Util/CryptTest.cpp Util/TimeStampTest.cpp)

target_link_libraries(Test
        uv
        gtest
        glog
        uuid
        mysqlcppconn
        ssl
        crypto
        Util
        uvw)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/glog/include)
include_directories(../Util)
include_directories(../uvw)