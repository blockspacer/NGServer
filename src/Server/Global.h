#ifndef MASSGATE_MOD_GLOBAL_H
#define MASSGATE_MOD_GLOBAL_H


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

    // key:    fd
    // value:  1
    static std::map<int, uvw::TcpHandle *> fd_tcp;

    // key:    tcp handle
    // value:  user id
    static std::map<uvw::TcpHandle *, int> client_user;

    // key:    user id
    // value:  tcp handle
    static std::map<int, uvw::TcpHandle *> user_client;

    // key:   user id
    // value: is login
    static std::map<int, bool> user_auth;

    // read message worker queue
    static MessageQueue<std::shared_ptr<Worker>> read_message_queue;
    static Consumer read_consumer;

    // write message worker queue
    static MessageQueue<std::shared_ptr<Worker>> write_message_queue;
    static Consumer write_consumer;

    // user cube data
    static std::map<int, ProtoMsg::Message> user_cube;

    // user match state
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


#endif //MASSGATE_MOD_GLOBAL_H
