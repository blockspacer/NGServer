#include "MessageQueue.h"


std::string Worker::GetMessage() const {
    return msg_;
}

ProtoMsg::Message Worker::GetProtoMessage() const {
    return proto_msg_;
}

void Consumer::RunRead() {
    while (m_queue_.Size() > 0) {
        auto worker = m_queue_.Remove();
        auto *proto_msg = new ProtoMsg::Message;
        *proto_msg = worker->GetProtoMessage();
        ProtoBaseHandle::Execute(*proto_msg);
        delete proto_msg;
    }
}


void Consumer::RunWrite() {
    while (m_queue_.Size() > 0) {
        auto worker = m_queue_.Remove();
        auto *proto_msg = new ProtoMsg::Message;
        *proto_msg = worker->GetProtoMessage();
        ProtoBaseHandle::Execute(*proto_msg);
        delete proto_msg;
    }
}