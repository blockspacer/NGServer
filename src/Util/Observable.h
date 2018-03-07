#ifndef MASSGATE_MOD_OBSERVABLE_H
#define MASSGATE_MOD_OBSERVABLE_H


#include <vector>
#include <algorithm>
#include <cassert>

#include "Observer.h"

class Observable {
public:
    Observable() {
        obs_ = {};
    };

    virtual ~Observable() {

    };

    void AddObserver(Observer *o) {
        assert(o != nullptr);
        if (std::find(obs_.begin(), obs_.end(), o) == obs_.end()) {
            obs_.emplace_back(o);
        }
    }

    void DeleteObserver(Observer *o) {
        assert(o != nullptr);
        obs_.erase(std::remove(obs_.begin(), obs_.end(), o),
                   obs_.end());
    }

    void NotifyObservers() {
        for (auto const &x:obs_) {
            x->Update(this);
        }
    }

    void DeleteObservers() {
        obs_.clear();
    }

    void SetChanged() {
        changed_ = true;
    }

    bool HasChanged() {
        return changed_;
    }

    unsigned long CountObservers() {
        return obs_.size();
    }

private:
    bool changed_ = false;

    std::vector<Observer *> obs_;
};

#endif //MASSGATE_MOD_OBSERVABLE_H
