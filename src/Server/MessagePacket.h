#ifndef NGSERVER_MESSAGEPACK_H
#define NGSERVER_MESSAGEPACK_H


#include <Message.pb.h>
#include <StringConversion.h>

typedef struct MessageFormat {
    char *data;
    unsigned int length;

    ~MessageFormat() {
        delete[] data;
    }

} MessageFormat;

class MessagePacket {
public:
    static std::shared_ptr<MessageFormat> Pack(const ProtoMsg::Message &msg) {
        std::string data;
        msg.SerializeToString(&data);
        auto *message = new char[data.length() + 4];
        Util::PackMessage(data, message);
        auto mf = std::make_shared<MessageFormat>();
        mf->data = message;
        mf->length = (unsigned int) data.length() + 4;
        return mf;
    }

    static std::string UnPack() {
        return "";
    }
};


#endif //NGSERVER_MESSAGEPACK_H
