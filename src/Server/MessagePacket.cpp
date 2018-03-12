#include "MessagePacket.h"


std::shared_ptr<MessageFormat> MessagePacket::Pack(const ProtoMsg::Message &msg) {
    std::string data;
    msg.SerializeToString(&data);
    auto *message = new char[data.length() + 4];
    Util::PackMessage(data, message);
    auto mf = std::make_shared<MessageFormat>();
    mf->data = message;
    mf->length = (unsigned int) data.length() + 4;
    return mf;
}

std::string MessagePacket::UnPack() {
    return "";
}