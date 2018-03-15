link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_executable(FSM
        _FSM.cpp
        Heroine.cpp
        Heroine.h)

target_link_libraries(FSM
        gtest
        glog
        Util
        pthread)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/protobuf/include)
include_directories(../Util)
