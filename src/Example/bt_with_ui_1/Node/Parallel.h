#ifndef NGSERVER_PARALLEL_H
#define NGSERVER_PARALLEL_H


#include <string>
#include <vector>
#include "../Node.h"
#include "../ParentNode.h"

/**
 * @brief Runs childs nodes in parallel.
 */
class Parallel: public ParentNode {
public:
    explicit Parallel(std::string name,
                      int num_req_fail,
                      int num_req_success) {
        name_ = name;
        num_required_to_fail_ = num_req_fail;
        num_required_to_success_ = num_req_success;
    }

    Status Tick(TimeData time) {
        int num_children_success = 0;
        int num_children_failed = 0;
        for (auto child:children_) {
            auto child_status = child.tick(time);
            switch (child_status) {
                case Status::Success:
                    ++num_children_success;
                    break;
                case Status::Failure:
                    ++num_children_failed;
                default:
                    break;
            }
        }
        if (num_children_success > 0 &&
            num_children_success >= num_required_to_success_) {
            return Status::Success;
        }
        if (num_children_failed > 0 &&
            num_children_failed > num_required_to_fail_) {
            return Status::Failure;
        }
        return Status::Running;
    }

    void addChild(Node child) {
        children_.push_back(child);
    }

private:
    std::string name_;
    std::vector<Node> children_;
    int num_required_to_fail_;
    int num_required_to_success_;
};


#endif //NGSERVER_PARALLEL_H
