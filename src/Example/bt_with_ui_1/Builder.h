#ifndef NGSERVER_BUILDER_H
#define NGSERVER_BUILDER_H

#include <stack>
#include <string>
#include <cassert>

#include "Node.h"
#include "ParentNode.h"
#include "Node/Action.h"
#include "Node/Inverter.h"
#include "Node/Selector.h"
#include "Node/Sequence.h"

typedef Status (*Func)(TimeData);

/**
 * @brief Api for building a behavior tree.
 */
class Builder {
public:
    /**
     * @brief Create an action node.
     * @param name
     * @param fn
     * @return
     */
    Builder doit(std::string name, Func fn) {
        assert(parent_node_stack_->size() > 0 &&
               "Can't create an unnested ActionNode, it must be a leaf node.");
        auto action_node = Action(name, fn);
        parent_node_stack_->top().addChild(action_node);
        return *this;
    }

    /**
     * @brief Like an action node. but the function can return
     *        true/false and is mapped to success/failure.
     * @param name
     * @param fn
     * @return
     */
    Builder condition(std::string name, Func fn) {
        return doit(name, fn);
    }

    /**
     * @brief Create an inverter node that inverts the success/failure of its children.
     * @param name
     * @return
     */
    Builder inverter(std::string name) {
        auto inverter_node = Inverter(name);
        if (parent_node_stack_->size() > 0) {
            parent_node_stack_->top().addChild(inverter_node);
        }
        parent_node_stack_->push(inverter_node);
        return *this;
    }

    Builder sequence(std::string name) {
        auto sequence_node = Sequence(name);
        if (parent_node_stack_->size() > 0) {
            parent_node_stack_->top().addChild(sequence_node);
        }
        parent_node_stack_->push(sequence_node);
        return *this;
    }

    Builder selector(std::string name) {
        auto selector_node = Selector(name);
        if (parent_node_stack_->size() > 0) {
            parent_node_stack_->top().addChild(selector_node);
        }
        parent_node_stack_->push(selector_node);
        return *this;
    }

    /**
     * @brief Splice a sub tree into the parent tree.
     * @param sub_tree
     * @return
     */
    Builder splice(Node *sub_tree) {
        assert(sub_tree != nullptr &&
               "subTree");
        assert(parent_node_stack_->size() > 0 &&
               "Can't splice an unnested sub-tree, there must be a parent-tree.");
        parent_node_stack_->top().addChild(*sub_tree);
        return *this;
    }

    /**
     * @brief Build the actual tree.
     * @return
     */
    Node build() {
        assert (cur_node_ != nullptr &&
                "Can't create a behaviour tree with zero nodes");
        return *cur_node_;
    }

    /**
     * @brief Ends a sequence of children.
     * @return
     */
    Builder end() {
        cur_node_ = &parent_node_stack_->top();
        parent_node_stack_->pop();
        return *this;
    }

private:
    Node *cur_node_;
    std::stack<ParentNode> *parent_node_stack_;
};


#endif //NGSERVER_BUILDER_H
