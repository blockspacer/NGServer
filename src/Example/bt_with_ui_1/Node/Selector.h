#ifndef NGSERVER_SELECTOR_H
#define NGSERVER_SELECTOR_H


#include <string>
#include <vector>

#include "../Node.h"
#include "../ParentNode.h"

class Selector : public ParentNode {
public:
    explicit Selector(std::string name) {
        name_ = name;
    }

    Status tick(TimeData time) {
        for (auto child :children_) {
            auto child_status = child.tick(time);
            if (child_status != Status::Failure) {
                return child_status;
            }
        }
        return Status::Failure;
    }

    void addChild(Node child) {
        children_.push_back(child);
    }

private:
    std::string name_;
    std::vector<Node> children_;
};


#endif //NGSERVER_SELECTOR_H
