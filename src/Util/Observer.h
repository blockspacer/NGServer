#ifndef NGSERVER_OBSERVER_H
#define NGSERVER_OBSERVER_H

class Observable;

class Observer {
public:
    Observer() = default;

    ~Observer() = default;

    virtual void Update(Observable *o)=0;
};


#endif //NGSERVER_OBSERVER_H
