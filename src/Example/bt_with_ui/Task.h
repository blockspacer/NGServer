#ifndef NGSERVER_TASK_H
#define NGSERVER_TASK_H


#include "Base.h"

class Task : public Node {
public:
    ~Task() override = default;
};

class MoveTo : public Task {

};

class PlaySound : public Task {

};

class RunBehavior : public Task {

};

class Wait : public Task {

};

class WaitBlackboardTime : public Task {

};

#endif //NGSERVER_TASK_H
