#ifndef MASSGATE_MOD_OBSERVER_H
#define MASSGATE_MOD_OBSERVER_H

class Observable;

class Observer {
public:
    Observer() = default;

    ~Observer() = default;

    virtual void Update(Observable *o)=0;
};


#endif //MASSGATE_MOD_OBSERVER_H
