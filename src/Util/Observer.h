#ifndef NGSERVER_OBSERVER_H
#define NGSERVER_OBSERVER_H


#include "../Proto/Message.pb.h"

namespace Util {

class Observer {
public:
    virtual ~Observer() = default;

    virtual void OnNotify(const ProtoMsg::Message &msg) = 0;
};

class Subject {
public:
    void AddObserver(Observer *obs) {
        if (std::find(observers_.begin(), observers_.end(), obs) == observers_.end()) {
            observers_.emplace_back(obs);
        }
    }

    void RemoveObserver(Observer *obs) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), obs),
                         observers_.end());
    }

    void DeleteObservers() {
        observers_.clear();
    }

    unsigned long CountObservers() const {
        return observers_.size();
    }

protected:
    void Notify(const ProtoMsg::Message &msg) {
        for (auto &x:observers_) {
            x->OnNotify(msg);
        }
    }

private:
    std::vector<Observer *> observers_;
};

}


#endif //NGSERVER_OBSERVER_H
