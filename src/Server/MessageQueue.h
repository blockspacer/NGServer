#ifndef NGSERVER_MESSAGEQUEUE_H
#define NGSERVER_MESSAGEQUEUE_H


#include <pthread.h>
#include <list>
#include <queue>
#include <string>
#include <fstream>

#include <glog/logging.h>

#include "../Proto/Message.pb.h"
#include "ProtoHandle/ProtoBaseHandle.h"

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

    int Size() const;

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

    std::string GetMessage() const;

    ProtoMsg::Message GetProtoMessage() const;

private:
    std::string msg_;
    ProtoMsg::Message proto_msg_;
};

class Consumer {
public:
    explicit Consumer(MessageQueue<std::shared_ptr<Worker>> &queue)
            : m_queue_(queue) {}

    void RunRead();

    void RunWrite();

private:
    MessageQueue<std::shared_ptr<Worker>> &m_queue_;
};


#endif //NGSERVER_MESSAGEQUEUE_H
