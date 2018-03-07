link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

file(GLOB uvw_src
        "*.hpp"
        "*.cpp")

add_library(uvw
        ${uvw_src})

target_link_libraries(uvw
        uv
        gtest
        glog)

include_directories(/usr/local/lib/glog/include)