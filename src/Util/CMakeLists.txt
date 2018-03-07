link_directories(/usr/local/lib/libuv/)
link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

add_library(Util
        RunTime.cpp
        RunTime.h
        StringConversion.h
        StringConversion.cpp
        UUID.cpp
        UUID.h
        Observable.cpp
        Observable.h
        Observer.cpp
        Observer.h
        Crypt.cpp
        Crypt.h
        Timeable.cpp
        Timeable.h TimeStamp.cpp TimeStamp.h)

target_link_libraries(Util
        uv
        gtest
        glog
        ssl
        pthread)

include_directories(/usr/local/lib/glog/include)