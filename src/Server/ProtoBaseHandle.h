#ifndef MASSGATE_MOD_PROTOBASEHANDLE_H
#define MASSGATE_MOD_PROTOBASEHANDLE_H


#include <map>

#include <glog/logging.h>
#include <gtest/gtest.h>

#include "../Proto/Message.pb.h"

class ProtoBaseHandle {
public:
    ProtoBaseHandle()
            : type_(ProtoMsg::Unknow) {};

    explicit ProtoBaseHandle(ProtoMsg::MSG type)
            : type_(type) {
        Register(this);
    }

    virtual ~ProtoBaseHandle() = default;

    ProtoMsg::MSG GetType() const {
        return type_;
    }

    void Register(ProtoBaseHandle *h);

    virtual void Process(const ProtoMsg::Message &msg) = 0;

    static void Execute(const ProtoMsg::Message &msg);

private:
    ProtoMsg::MSG type_;
    static std::map<ProtoMsg::MSG, ProtoBaseHandle *> handles_;
};


template<ProtoMsg::MSG TYPE>
class ProtoMessageHandle : public ProtoBaseHandle {
public:
    ProtoMessageHandle() : ProtoBaseHandle(TYPE) {};

    ~ProtoMessageHandle() override = default;

    void Process(const ProtoMsg::Message &msg) override;

private:
    static ProtoMessageHandle handle_;
};


#endif //MASSGATE_MOD_PROTOBASEHANDLE_H
