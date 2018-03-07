#include <uuid/uuid.h>
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>

#include "CubeServer.h"

CubeServer::CubeServer() {
    uuid_generate(uuid_);
    char out[36];
    memset(out, 0x00, sizeof(out));
    uuid_unparse_lower(uuid_, out);
    std::string str(out);
    uuid_string_ = out;
}

CubeServer::~CubeServer() {

}

std::string CubeServer::GetName() {
    return name_;
}

uuid_t *CubeServer::GetId() {
    return &uuid_;
}

std::string CubeServer::GetIdString() {
    return uuid_string_;
}

void CubeServer::SetEnable(bool value) {
    enable_ = value;
}

bool CubeServer::GetEnable() {
    return enable_;
}