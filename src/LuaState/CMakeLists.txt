link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)
link_directories(/home/me/CLionProjects/NGServer/cmake-build-debug-clang/src/LuaState)

add_library(mylib SHARED
        LuaMyLib.cpp)

target_link_libraries(mylib
        gtest
        glog
        pthread
        lua5.3)

SET_TARGET_PROPERTIES(mylib PROPERTIES LINKER_LANGUAGE CXX)

#add_executable(LuaState
#        main.cpp
#        LoadLuaFile.cpp
#        LoadLuaFile.h)
#
#target_link_libraries(LuaState
#        gtest
#        glog
#        pthread
#        lua5.3
#        mylib)

include_directories(/usr/local/lib/glog/include)
include_directories(/usr/include/lua5.3)