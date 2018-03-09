#ifndef NGSERVER_GLOBAL_H
#define NGSERVER_GLOBAL_H


#include <map>
#include <memory>
#include <sys/time.h>

#include "../uvw/uvw.hpp"
#include "MessageQueue.h"

class GlobalData {
public:
    typedef struct SceneUsers {
        int one;
        int two;
    } SceneUsers;

    static std::map<int, uvw::TcpHandle *> fd_tcp;

    static std::map<uvw::TcpHandle *, int> client_user;

    static std::map<int, uvw::TcpHandle *> user_client;

    static std::map<int, bool> user_auth;

    static MessageQueue<std::shared_ptr<Worker>> read_message_queue;
    static Consumer read_consumer;

    static MessageQueue<std::shared_ptr<Worker>> write_message_queue;
    static Consumer write_consumer;

    static std::map<int, int> user_match;

    // user game scene
    static std::map<int, int> user_scene;
    static std::map<int, SceneUsers> scene_user;
    static int scene_id; // increase

    // user game scene
    static std::map<int, int> scene_time;

    // scene data calculate
    static std::map<int, std::map<int, ProtoMsg::Message>> scene_data;

    static void WavesCreate();

};


#endif //NGSERVER_GLOBAL_H
