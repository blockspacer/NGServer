#ifndef MASSGATE_MOD_TG_UUID_H
#define MASSGATE_MOD_TG_UUID_H


#include <uuid/uuid.h>
#include <cstring>
#include <string>

namespace Util {

class UUID {
public:

    static void GetUuid(uuid_t &uuid) {
        uuid_generate(uuid);
    }

    static std::string GetUuidString() {
        std::string uuid_string = "";
        uuid_t uuid = {};
        uuid_generate(uuid);
        char out[36];
        memset(out, 0x00, sizeof(out));
        uuid_unparse_lower(uuid, out);
        std::string str(out);
        uuid_string = out;
        return uuid_string;
    }
};

}


#endif //MASSGATE_MOD_TG_UUID_H
