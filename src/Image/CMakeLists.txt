link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_executable(Image
        Temp.cpp _Image.cpp)

target_link_libraries(Image
        gtest
        glog
        Util
        pthread
        glfw3
        GL
        X11
        dl)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/local/lib/protobuf/include)
include_directories(../Util)
