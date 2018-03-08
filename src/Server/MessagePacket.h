#ifndef NGSERVER_MESSAGEPACK_H
#define NGSERVER_MESSAGEPACK_H


#include <Message.pb.h>
#include <StringConversion.h>

typedef struct MessageFormat {
    char *message;
    unsigned long length;

    ~MessageFormat() {
        delete[] message;
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
        mf->message = message;
        mf->length = data.length() + 4;
        return mf;
    }

    static void UnPack() {

    }
};


#endif //NGSERVER_MESSAGEPACK_H
