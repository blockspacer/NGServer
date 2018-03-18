#ifndef NGSERVER_COMPOSITE_H
#define NGSERVER_COMPOSITE_H

#include "Base.h"

class Composite : public Node {
public:
    ~Composite() override = default;
};

class Selector : public Composite {

};

class Sequence : public Composite {

};

class Parallel : public Composite {

};

#endif //NGSERVER_COMPOSITE_H
