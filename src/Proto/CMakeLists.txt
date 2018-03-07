link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)
link_directories(/usr/local/lib/protobuf/lib)
link_directories(/usr/local/lib/uvw)

add_executable(Proto
        _Proto.cpp
        MessageTest.cpp
        Message.pb.h
        Message.pb.cc
        )

target_link_libraries(Proto
        uv
        gtest
        glog
        protobuf
        uuid
        Util
        pthread)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/protobuf/include)
include_directories(../Util)
include_directories(../Proto)