/**
 * @brief Converts anything to anything
 */

#ifndef NGSERVER_STRINGUTIL_H
#define NGSERVER_STRINGUTIL_H


#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <cstring>

namespace Util {

/**
 * @brief Conversions from string types to char pointer types.
 * @param str
 * @return
 */
char *StringToChar(std::string &str);

/**
 * @brief Conversions from string types to integral types.
 * @param str
 * @return
 */
int StringToInt(std::string str);

/**
 * @brief Conversions from char[4] types to int32 types.
 * @param buf
 * @param val
 */
void SerializeInt32(char (&buf)[4], int32_t val);

/**
 * @brief Conversions from int32 types to char[4] types.
 * @param buf
 * @param val
 */
int32_t ParseInt32(const char (&buf)[4]);

/**
 * @brief Conversions from string to char*,
 *        add 4 bytes message length ( message length + 4) to the begin
 * @param data
 * @param message
 * @return
 */
char *PackMessage(std::string &data, char *message);

}


#endif /* NGSERVER_STRINGUTIL_H */