#ifndef MASSGATE_MOD_CUBESERVER_H
#define MASSGATE_MOD_CUBESERVER_H

#include <string>
#include <uuid/uuid.h>

class CubeServer {
public:
    CubeServer();

    ~CubeServer();

    std::string GetName();

    std::string GetIdString();

    uuid_t *GetId();

    void SetEnable(bool value);

    bool GetEnable();

private:
    float x_;
    float y_;
    float z_;
    bool enable_;

    std::string name_;
    std::string uuid_string_;
    uuid_t uuid_;
};


#endif //MASSGATE_MOD_CUBESERVER_H
