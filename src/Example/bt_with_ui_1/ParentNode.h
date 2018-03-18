#ifndef NGSERVER_TREENODE_H
#define NGSERVER_TREENODE_H


#include "Node.h"

/**
 * @brief Interface for behaviour tree nodes.
 */
class ParentNode : public Node {
public:
    /**
     * @brief Add a child to the parent node.
     * @param child
     */
    virtual void addChild(Node child) {};
};


#endif //NGSERVER_TREENODE_H
