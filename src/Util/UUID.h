#ifndef NGSERVER_TG_UUID_H
#define NGSERVER_TG_UUID_H


#include <uuid/uuid.h>
#include <cstring>
#include <string>

namespace Util {

class UUID {
public:
    /**
     * @brief Get uuid, eg: ec981df5-3909-46ad-af6e-a8df56273db3
     * @param out
     */
    static void GetUuidString(std::string &out) {
        uuid_t uuid = {};
        uuid_generate(uuid);
        char in[36];
        memset(in, 0x00, sizeof(in));
        uuid_unparse_lower(uuid, in);
        std::string str(in);
        out = in;
    }
};

}


#endif //NGSERVER_TG_UUID_H
