link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)
link_directories(/usr/local/lib/uvw)

# build tests
set(BUILD_TESTS 1)
# build benchmarks
set(BUILD_BENCHMARKS 1)

option(BUILD_TESTS "If enabled, compile the tests." OFF)
option(BUILD_BENCHMARKS "If enabled, compile the benchmarks." OFF)

# add_definitions(-DNDEBUG)

file(GLOB test
        "*/*Test.cpp")

file(GLOB benchmark
        "*/*Benchmark.cpp")

add_executable(Test
        _Test.cpp)

if (BUILD_TESTS)
    target_sources(Test
            PUBLIC ${test})
endif ()

if (BUILD_BENCHMARKS)
    target_sources(Test
            PUBLIC ${benchmark})
endif ()

target_link_libraries(Test
        uv
        gtest
        glog
        benchmark
        uuid
        mysqlcppconn
        ssl
        pthread
        crypto
        Util
        uvw)

include_directories(/usr/local/lib/glog/include)
include_directories(../Util)
include_directories(../uvw)