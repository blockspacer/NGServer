#ifndef NGSERVER_CRYPT_H
#define NGSERVER_CRYPT_H


#include <iostream>
#include <string>
#include <cstring>
#include <openssl/sha.h>
#include <openssl/md5.h>

namespace Util {

class Crypt {
public:
    /**
     * @brief Get sha1 lower case string
     * @param in
     * @param out
     */
    static void GetSha1(std::string &in, std::string &out) {
        SHA_CTX ctx = {0};
        unsigned char hash[SHA_DIGEST_LENGTH];
        char hashc[SHA_DIGEST_LENGTH];

        const unsigned char *s3 = new unsigned char[in.size() + 1];;
        strcpy((char *) s3, in.c_str());

        SHA1_Init(&ctx);
        SHA1_Update(&ctx, s3, in.size());
        SHA1_Final(hash, &ctx);

        int i = 0;
        for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2x", (int) hash[i]);
        }

        out = (std::string) hashc;
    }

    /**
     * @brief Another method to get sha1 lower case string
     * @param in
     * @param out
     */
    static void DebugGetSha1(std::string &in, std::string &out) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        char hashc[SHA_DIGEST_LENGTH];

        const unsigned char *s3 = new unsigned char[in.length() + 1];
        strcpy((char *) s3, in.c_str());

        SHA1(s3, in.length(), hash);

        int i = 0;
        for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2x", (int) hash[i]);
        }

        out = (std::string) hashc;
    }

    /**
     * @brief Get md5 lower case string
     * @param in
     * @param out
     */
    static void GetMD5(std::string &in, std::string &out) {
        MD5_CTX ctx = {0};
        unsigned char hash[MD5_DIGEST_LENGTH];
        char hashc[MD5_DIGEST_LENGTH];

        const unsigned char *s3 = new unsigned char[in.length() + 1];;
        strcpy((char *) s3, in.c_str());

        MD5_Init(&ctx);
        MD5_Update(&ctx, s3, in.length());
        MD5_Final(hash, &ctx);

        int i = 0;
        for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2x", (int) hash[i]);
        }

        out = (std::string) hashc;
    }

    /**
     * @todo the code result[MD5_DIGEST_LENGTH + 1], if not +1 will cause error invalid pointer
     * @brief Another method to get md5 string
     * @param in
     * @param out
     */
    static void DebugGetMD5(std::string &in, std::string &out) {
        unsigned char result[MD5_DIGEST_LENGTH + 1] = {0};
        char hashc[MD5_DIGEST_LENGTH] = {0};

        const unsigned char *s3 = new unsigned char[in.length() + 1];
        strcpy((char *) s3, in.c_str());

        MD5(s3, 3, result);

        int i = 0;
        for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2x", result[i]);
        }

        out = (std::string) hashc;

    }
};

}


#endif //NGSERVER_CRYPT_H
