link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_executable(BTree
        _BTree.cpp)

target_link_libraries(BTree
        gtest
        glog
        Util
        pthread)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/protobuf/include)
include_directories(../Util)
