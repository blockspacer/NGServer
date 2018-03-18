#ifndef NGSERVER_SEQUENCE_H
#define NGSERVER_SEQUENCE_H


#include <string>
#include <vector>

#include "../Node.h"
#include "../ParentNode.h"

class Sequence :public ParentNode{
public:
    explicit Sequence(std::string name) {
        name_ = name;
    }

    Status tick(TimeData time) {
        for (auto child :children_) {
            auto child_status = child.tick(time);
            if (child_status != Status::Success) {
                return child_status;
            }
        }
        return Status::Success;
    }

    void addChild(Node child) {
        children_.push_back(child);
    }

private:
    std::string name_;
    std::vector<Node> children_;
};


#endif //NGSERVER_SEQUENCE_H
