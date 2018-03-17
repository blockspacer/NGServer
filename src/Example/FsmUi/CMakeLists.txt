link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_executable(FsmUi
        _FsmUi.cpp
        Heroine.h
        Heroine.cpp
        libpng_test.cpp Fsm.h)

target_link_libraries(FsmUi
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


