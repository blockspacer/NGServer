#include "RunTime.h"

#include <sys/time.h>
#include <cstdio>
#include <iostream>

namespace Util {

RunTime::RunTime() {

}

RunTime::~RunTime() {

}

void RunTime::Start() {
    gettimeofday(&start_, nullptr);
}

void RunTime::End() {
    gettimeofday(&end_, nullptr);
}

long double RunTime::GetRunTime() {

    long double mtime, seconds, useconds;

    seconds = end_.tv_sec - start_.tv_sec;
    useconds = end_.tv_usec - start_.tv_usec;

    mtime = seconds + useconds / 1000.0 / 1000.0;

    return mtime;
}

timeval RunTime::GetStartTime() {
    return start_;
}

timeval RunTime::GetEndTime() {
    return end_;
}

}
