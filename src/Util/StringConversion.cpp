#include "StringConversion.h"

namespace Util {

template<typename Out>
void Split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> Split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    Util::Split(s, delim, std::back_inserter(elems));
    return elems;
}

// string to char*
char *Str2char(std::string &str) {
    auto cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}

// string to int
int Str2int(std::string s) {
    return std::stoi(s);
}

void SerializeInt32(char (&buf)[4], int32_t val) {
    uint32_t uval = val;
    buf[0] = uval;
    buf[1] = uval >> 8;
    buf[2] = uval >> 16;
    buf[3] = uval >> 24;
}

int32_t ParseInt32(const char (&buf)[4]) {
    // This prevents buf[i] from being promoted to a signed int.
    uint32_t u0 = buf[0], u1 = buf[1], u2 = buf[2], u3 = buf[3];
    uint32_t uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
    return uval;
}

char *PackMessage(std::string &data, char *message) {
    char buf[4];
    int32_t header = 4 + (int) data.length();
    SerializeInt32(buf, header);

    for (int i = 0; i < 4; ++i) {
        message[i] = buf[i];
    }
    for (int i = 0; i < data.length(); ++i) {
        message[i + 4] = data[i];
    }
    return message;
}

}