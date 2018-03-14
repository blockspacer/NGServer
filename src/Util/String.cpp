#include <sstream>

#include "String.h"

namespace Util {

/**
 * @brief Split a string into a list of tokens by delimiter.
 *
 * @tparam Out
 * @param s
 * @param delim
 * @param result
 *
 * @example
 *
 *    std::vector<std::string> result;
 *    Util::Split("hello world", ' ', result);
 *
 *    std::vector<std::string> result;
 *    result = Util::Split("hello world",' ');
 */

template<typename Out>
void Split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> Split(const std::string &str, char delim) {
    std::vector<std::string> elems;
    Util::Split(str, delim, std::back_inserter(elems));
    return elems;
}

}