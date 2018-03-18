#ifndef NGSERVER_DECORATOR_H
#define NGSERVER_DECORATOR_H


#include "Base.h"

class Decorator : public Node {
public:
    ~Decorator() override = default;
};

class Blackboard : public Decorator {

};

class TimeLimit : public Decorator {

};


#endif //NGSERVER_DECORATOR_H
