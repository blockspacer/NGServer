link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)
link_directories(/usr/local/lib/protobuf/lib)
link_directories(/usr/local/lib/uvw)

#add_definitions(-DNDEBUG)

add_executable(Server
        _Server.cpp
        ../Proto/Message.pb.cc
        ../Proto/Message.pb.h
        CubeServer.cpp
        CubeServer.h
        ProtoBaseHandle.cpp
        ProtoBaseHandle.h
        DatabaseInstance.cpp
        DatabaseInstance.h
        MessageQueue.cpp
        MessageQueue.h
        Global.h
        Global.cpp User.cpp User.h Connector.cpp Connector.h MessagePacket.cpp MessagePacket.h)

target_link_libraries(Server
        uv
        gtest
        glog
        protobuf
        mysqlcppconn
        uuid
        pthread
        Util
        uvw)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/protobuf/include)
include_directories(../Util)
include_directories(../Proto)
include_directories(../uvw)