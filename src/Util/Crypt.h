#ifndef MASSGATE_MOD_CRYPT_H
#define MASSGATE_MOD_CRYPT_H

#include <iostream>
#include <string>
#include <cstring>
#include <openssl/sha.h>
#include <openssl/md5.h>

namespace Util {

class Crypt {
public:
    // uppercase
    static std::string GetSha1(std::string s) {
        SHA_CTX ctx = {0};
        unsigned char hash[SHA_DIGEST_LENGTH];
        char hashc[SHA_DIGEST_LENGTH];
        int i = 0;

        const unsigned char *s3 = new unsigned char[s.size() + 1];;
        strcpy((char *) s3, s.c_str());

        SHA1_Init(&ctx);
        SHA1_Update(&ctx, s3, s.size());
        SHA1_Final(hash, &ctx);

        for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2x", (int) hash[i]);
        }

        return (std::string) hashc;
    }

    static std::string DebugGetSha1(std::string s) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        char hashc[SHA_DIGEST_LENGTH];

        const unsigned char *s3 = new unsigned char[s.size() + 1];
        strcpy((char *) s3, s.c_str());

        SHA1(s3, s.size(), hash);

        int i = 0;
        for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2x", (int) hash[i]);
        }

        return (std::string) hashc;
    }

    // lower case
    static std::string GetMD5(std::string s) {
        MD5_CTX ctx = {0};
        unsigned char outmd[MD5_DIGEST_LENGTH];
        char hashc[MD5_DIGEST_LENGTH];
        int i = 0;

        const unsigned char *s3 = new unsigned char[s.size() + 1];;
        strcpy((char *) s3, s.c_str());

        MD5_Init(&ctx);
        MD5_Update(&ctx, s3, 3);
        MD5_Final(outmd, &ctx);

        for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
            sprintf(&hashc[i * 2], "%.2X", outmd[i]);
        }

        return ((std::string) hashc);
    }

    static std::string DebugGetMD5(std::string s) {
        unsigned char hash[MD5_DIGEST_LENGTH];
        char hashc[MD5_DIGEST_LENGTH];
//
//        const unsigned char *s3 = new unsigned char[s.size() + 1];
//        strcpy((char *) s3, s.c_str());
//
//        MD5(s3, s.size(), hash);
//
//        for (int i = 0; i < 16; i++) {
//            sprintf(&hashc[i * 2], "%.2x", (int) hash[i]);
//        }

        unsigned char data[] = "abc";
        unsigned char md[16];
        int i;
        char tmp[3] = {};
        char buf[16] = {};
        MD5(data, 3, md);
        for (i = 0; i < 16; i++) {
            printf("%.2x", md[i]);
        }
        printf("\n");

        return (std::string) "";
    }
};

}


#endif //MASSGATE_MOD_CRYPT_H
