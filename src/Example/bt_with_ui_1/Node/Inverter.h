#ifndef NGSERVER_INVERTER_H
#define NGSERVER_INVERTER_H


#include <string>
#include <cassert>

#include "../Status.h"
#include "../TimeData.h"
#include "../Node.h"
#include "../ParentNode.h"

/**
 * @brief Decorator node that inverts the success/failure of its child.
 */
class Inverter : public ParentNode {
public:
    explicit Inverter(std::string name) {
        name_ = name;
    }

    Status Tick(TimeData time) {
        assert(child_node_ != nullptr &&
               "InverterNode must have a child node!");
        auto result = child_node_->tick(time);
        if (result == Status::Failure) {
            return Status::Success;
        } else if (result == Status::Success) {
            return Status::Failure;
        } else {
            return result;
        }
    }

    /**
     * @brief Add a child to the parent node.
     */
    void addChild(Node *child) {
        assert(child_node_ != nullptr &&
               "Can't add more than a single child to InverterNode!");
        child_node_ = child;
    }

private:
    std::string name_;
    Node *child_node_;
};


#endif //NGSERVER_INVERTER_H
