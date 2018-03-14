#ifndef NGSERVER_STRING_H
#define NGSERVER_STRING_H


#include <string>
#include <vector>

namespace Util {

template<typename Out>
void Split(const std::string &s, char delim, Out result);

std::vector<std::string> Split(const std::string &s, char delim);


}


#endif //NGSERVER_STRING_H
