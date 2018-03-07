#ifndef NGSERVER_STRINGUTIL_H
#define NGSERVER_STRINGUTIL_H


#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <cstring>

namespace Util {

template<typename Out>
void Split(const std::string &s, char delim, Out result);

std::vector<std::string> Split(const std::string &s, char delim);

// string to char*
char *Str2char(std::string &str);

// string to int
int Str2int(std::string s);


void SerializeInt32(char (&buf)[4], int32_t val);

int32_t ParseInt32(const char (&buf)[4]);

char *PackMessage(std::string &data, char *message);

}


#endif /* NGSERVER_STRINGUTIL_H */