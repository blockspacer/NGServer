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
    static std::shared_ptr<MessageFormat> Pack(const ProtoMsg::Message &msg);

    static std::string UnPack();
};


#endif //NGSERVER_MESSAGEPACK_H
