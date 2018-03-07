#ifndef NGSERVER_MESSAGEQUEUE_H
#define NGSERVER_MESSAGEQUEUE_H


#include <pthread.h>
#include <list>
#include <queue>
#include <string>
#include <fstream>

#include <glog/logging.h>

#include "../Proto/Message.pb.h"
#include "ProtoBaseHandle.h"


// TODO phtead safe
template<typename T>
class MessageQueue {
public:
    MessageQueue() : m_queue_() {};

    ~MessageQueue() = default;

    void Add(T item) {
        m_queue_.push(item);
    }

    T Remove() {
        T item = m_queue_.front();
        m_queue_.pop();
        return item;
    }

    T Front() {
        T item = m_queue_.front();
        return item;
    }

    int Size() const {
        auto size = (int) m_queue_.size();
        return size;
    }

private:
    std::queue<T> m_queue_;
};

class Worker {
public:
    explicit Worker(std::string &msg)
            : msg_(msg) {}

    explicit Worker(ProtoMsg::Message &proto_msg)
            : proto_msg_(proto_msg) {}

    ~Worker() = default;

    std::string GetMessage() const {
        return msg_;
    }

    ProtoMsg::Message GetProtoMessage() const {
        return proto_msg_;
    }

private:
    std::string msg_;
    ProtoMsg::Message proto_msg_;
};

class Consumer {
public:
    explicit Consumer(MessageQueue<std::shared_ptr<Worker>> &queue)
            : m_queue_(queue) {}

    void RunRead() {
        while (m_queue_.Size() > 0) {
            auto worker = m_queue_.Remove();
            auto *proto_msg = new ProtoMsg::Message;
            *proto_msg = worker->GetProtoMessage();
            ProtoBaseHandle::Execute(*proto_msg);
            delete proto_msg;
        }
    }

    void RunWrite() {
        while (m_queue_.Size() > 0) {
            auto worker = m_queue_.Remove();
            auto *proto_msg = new ProtoMsg::Message;
            *proto_msg = worker->GetProtoMessage();
            ProtoBaseHandle::Execute(*proto_msg);
            delete proto_msg;
        }
    }

private:
    MessageQueue<std::shared_ptr<Worker>> &m_queue_;
};


#endif //NGSERVER_MESSAGEQUEUE_H
