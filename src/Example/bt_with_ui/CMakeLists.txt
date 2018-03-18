link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_executable(bt_with_ui
        _bt_with_ui.cpp
        Base.cpp Base.h
        Composite.cpp Composite.h Task.cpp Task.h Decorator.cpp Decorator.h Services.cpp Services.h Root.cpp Root.h)

target_link_libraries(bt_with_ui
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


