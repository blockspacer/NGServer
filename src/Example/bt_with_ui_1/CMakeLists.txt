link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_executable(bt_with_ui_1
        _bt_with_ui_1.cpp
        Node.cpp Node.h TimeData.cpp TimeData.h ParentNode.cpp ParentNode.h Status.cpp Status.h Builder.cpp Builder.h Node/Action.cpp Node/Action.h Node/Inverter.cpp Node/Inverter.h Node/Parallel.cpp Node/Parallel.h Node/Selector.cpp Node/Selector.h Node/Sequence.cpp Node/Sequence.h)

target_link_libraries(bt_with_ui_1
        gtest
        glog
        Util
        pthread
        glfw3
        GL
        X11
        dl
        png)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/protobuf/include)
include_directories(../Util)
include_directories(../../../third_party)


