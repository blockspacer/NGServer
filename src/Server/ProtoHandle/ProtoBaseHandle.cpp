#include "ProtoBaseHandle.h"

#include <sys/time.h>
#include <fstream>
#include <TimeStamp.h>

#include "../Global.h"
#include "Conv.h"
#include "../DatabaseInstance.h"
#include "../App/User.h"
#include "../MessagePacket.h"

std::map<ProtoMsg::MSG, ProtoBaseHandle *> ProtoBaseHandle::handles_ = {};

void ProtoBaseHandle::Register(ProtoBaseHandle *h) {
    handles_[h->GetType()] = h;
}

void ProtoBaseHandle::Execute(const ProtoMsg::Message &msg) {
    auto it = handles_.find(msg.msg_type());
    if (it != handles_.end()) {
        it->second->Process(msg);
    } else {
        LOG(ERROR) << "message type " << msg.msg_type()
                   << " does not have process method";
        LOG(ERROR) << "message content " << msg.DebugString();
    }
}

// login
template<>
void ProtoMessageHandle<ProtoMsg::Login_Request>
::Process(const ProtoMsg::Message &msg) {
    int fd = msg.session_id();
    auto *tcp = GlobalData::fd_tcp.find(fd)->second;

    if (GlobalData::client_user.find(tcp) ==
        GlobalData::client_user.end()) {
        auto user = User(std::move(msg.request().login().name()),
                         std::move(msg.request().login().password()));
        int user_id = user.Auth();
        int result = 0;
        if (user_id > 0) {
            if (GlobalData::user_auth.find(user_id) == GlobalData::user_auth.end()) {
                GlobalData::client_user.insert(
                        std::pair<uvw::TcpHandle *, int>(tcp, user_id));
                GlobalData::user_client.insert(
                        std::pair<int, uvw::TcpHandle *>(user_id, tcp));
                if (GlobalData::user_auth.find(user_id) == GlobalData::user_auth.end()) {
                    GlobalData::user_auth.insert(std::pair<int, bool>(user_id, true));
                }
                LOG(INFO) << "current client count: " << GlobalData::client_user.size();
                result = 0; // auth success
            } else {
                result = 2; // auth failed repeat login
            }
        } else {
            result = 1; // auth failed name and password is not match
        }
        auto msg1 = new ProtoMsg::LoginResponse;
        msg1->set_result(result);
        auto msg2 = new ProtoMsg::Response;
        msg2->set_allocated_login(msg1);
        auto msg3 = new ProtoMsg::Message;
        msg3->set_msg_type(ProtoMsg::MSG::Login_Response);
        msg3->set_session_id(fd);
        msg3->set_userid(user_id);
        msg3->set_allocated_response(msg2);
        std::shared_ptr<Worker> worker(new Worker(*msg3));
        GlobalData::write_message_queue.Add(worker);
    }
}

template<>
ProtoMessageHandle<ProtoMsg::Login_Request>
        ProtoMessageHandle<ProtoMsg::Login_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Login_Response>
::Process(const ProtoMsg::Message &msg) {
    int fd = msg.session_id();
    auto *tcp = GlobalData::fd_tcp.find(fd)->second;
    auto message = MessagePacket::Pack(msg);
    tcp->write(message->data, message->length);
}

template<>
ProtoMessageHandle<ProtoMsg::Login_Response>
        ProtoMessageHandle<ProtoMsg::Login_Response>::handle_ = {};

// register
template<>
void ProtoMessageHandle<ProtoMsg::Register_Request>
::Process(const ProtoMsg::Message &msg) {
    auto user = User(std::move(msg.request().register_().name()),
                     std::move(msg.request().register_().password()),
                     std::move(msg.request().register_().email()),
                     std::move(msg.request().register_().phone()));

    if (!user.SignUpCheck()) {
        int fd = msg.session_id();
        auto msg1 = new ProtoMsg::RegisterResponse;
        msg1->set_result(1);
        auto msg2 = new ProtoMsg::Response;
        msg2->set_allocated_register_(msg1);
        auto msg3 = new ProtoMsg::Message;
        msg3->set_msg_type(ProtoMsg::MSG::Register_Response);
        msg3->set_session_id(fd);
        msg3->set_allocated_response(msg2);
        std::shared_ptr<Worker> worker(new Worker(*msg3));
        GlobalData::write_message_queue.Add(worker);
        return;
    }

    auto db = DBInstance::instance();
    std::stringstream ss;
    ss.str("");
    ss << "insert into user (name, password, email, phone ) values ( ";
    ss << "'" << msg.request().register_().name() << "',";
    ss << "'" << msg.request().register_().password() << "',";
    ss << "'" << msg.request().register_().email() << "',";
    ss << "'" << msg.request().register_().phone() << "'";
    ss << ");";
    db.GetQuery().Execute(ss.str());

    int fd = msg.session_id();
    auto msg1 = new ProtoMsg::RegisterResponse;
    msg1->set_result(0);
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_register_(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_msg_type(ProtoMsg::MSG::Register_Response);
    msg3->set_session_id(fd);
    msg3->set_allocated_response(msg2);
    std::shared_ptr<Worker> worker(new Worker(*msg3));
    GlobalData::write_message_queue.Add(worker);
}

template<>
ProtoMessageHandle<ProtoMsg::Register_Request>
        ProtoMessageHandle<ProtoMsg::Register_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Register_Response>
::Process(const ProtoMsg::Message &msg) {
    int fd = msg.session_id();
    auto *tcp = GlobalData::fd_tcp.find(fd)->second;
    auto message = MessagePacket::Pack(msg);
    tcp->write(message->data, message->length);
}

template<>
ProtoMessageHandle<ProtoMsg::Register_Response>
        ProtoMessageHandle<ProtoMsg::Register_Response>::handle_ = {};

// match begin
template<>
void ProtoMessageHandle<ProtoMsg::Match_Begin_Request>
::Process(const ProtoMsg::Message &msg) {
    GlobalData::user_match.insert(std::pair<int, int>(msg.userid(), 1));

    auto msg1 = new ProtoMsg::MatchBeginResponse;
    msg1->set_id(msg.request().match_begin().id());
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_match_begin(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_userid(msg.userid());
    msg3->set_msg_type(ProtoMsg::MSG::Match_Begin_Response);
    msg3->set_allocated_response(msg2);

    std::shared_ptr<Worker> worker(new Worker(*msg3));
    GlobalData::write_message_queue.Add(worker);
    delete msg3;
}

template<>
ProtoMessageHandle<ProtoMsg::Match_Begin_Request>
        ProtoMessageHandle<ProtoMsg::Match_Begin_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Match_Begin_Response>
::Process(const ProtoMsg::Message &msg) {
    std::string data("");
    msg.SerializeToString(&data);
    auto *tcp = GlobalData::user_client.find(msg.userid())->second;
    char message[data.length() + 4];
    Util::PackMessage(data, message);
    tcp->write(message, sizeof(message));

    // one player
    if (msg.response().match_begin().id() == 0) {
        LOG(INFO) << "One Player Game";
        auto msg1 = new ProtoMsg::MatchCompleteResponse;
        msg1->set_id(0);
        msg1->set_other_user_id(0);
        msg1->set_user_count(1);
        auto msg2 = new ProtoMsg::Response;
        msg2->set_allocated_match_complete(msg1);
        auto msg3 = new ProtoMsg::Message;
        msg3->set_userid(msg.userid());
        msg3->set_msg_type(ProtoMsg::MSG::Match_Complete_Response);
        msg3->set_allocated_response(msg2);
        std::string data1("");
        msg3->SerializeToString(&data1);
        char message1[data1.length() + 4];
        Util::PackMessage(data1, message1);
        tcp->write(message1, sizeof(message1));

        int self_user_id = GlobalData::user_match.find(msg.userid())->first;
        GlobalData::user_match.erase(msg.userid());
        int scene_id = ++GlobalData::scene_id;
        GlobalData::user_scene.insert(
                std::pair<int, int>(self_user_id, scene_id));

        GlobalData::SceneUsers users;
        users.one = self_user_id;
        users.two = 0;
        GlobalData::scene_user.insert(
                std::pair<int, GlobalData::SceneUsers>(scene_id, users));

        struct timeval tv = {};
        gettimeofday(&tv, nullptr);
        GlobalData::scene_time.insert(
                std::pair<int, int>(scene_id, tv.tv_sec));

        delete msg3;
        return;
    }

    // two player
    if (GlobalData::user_match.size() > 1) {
        LOG(INFO) << "Two Player Game";
        // save user 1 id then remove from map
        int self_user_id = GlobalData::user_match.find(msg.userid())->first;
        GlobalData::user_match.erase(msg.userid());

        for (auto &x : GlobalData::user_match) {
            // notify user 1
            auto msg1 = new ProtoMsg::MatchCompleteResponse;
            msg1->set_other_user_id((uint) x.first);
            msg1->set_id(0);
            msg1->set_user_count(2);
            auto msg2 = new ProtoMsg::Response;
            msg2->set_allocated_match_complete(msg1);
            auto msg3 = new ProtoMsg::Message;
            msg3->set_userid(msg.userid());
            msg3->set_msg_type(ProtoMsg::MSG::Match_Complete_Response);
            msg3->set_allocated_response(msg2);
            std::string data1("");
            msg3->SerializeToString(&data1);
            char message1[data1.length() + 4];
            Util::PackMessage(data1, message1);
            tcp->write(message1, sizeof(message1));

            // notify user 2
            auto msg11 = new ProtoMsg::MatchCompleteResponse;
            msg11->set_other_user_id((uint) self_user_id);
            msg11->set_id(1);
            msg11->set_user_count(2);
            auto msg12 = new ProtoMsg::Response;
            msg12->set_allocated_match_complete(msg11);
            auto msg13 = new ProtoMsg::Message;
            msg13->set_userid(msg.userid());
            msg13->set_msg_type(ProtoMsg::MSG::Match_Complete_Response);
            msg13->set_allocated_response(msg12);
            std::string data2("");
            msg13->SerializeToString(&data2);
            tcp = GlobalData::user_client.find(x.first)->second;
            char message2[data2.length() + 4];
            Util::PackMessage(data2, message2);
            tcp->write(message2, sizeof(message2));

            int scene_id = ++GlobalData::scene_id;
            GlobalData::user_scene.insert(
                    std::pair<int, int>(self_user_id, scene_id));
            GlobalData::user_scene.insert(
                    std::pair<int, int>(x.first, scene_id));

            GlobalData::SceneUsers users;
            users.one = self_user_id;
            users.two = x.first;
            GlobalData::scene_user.insert(
                    std::pair<int, GlobalData::SceneUsers>(scene_id, users));

            struct timeval tv = {};
            gettimeofday(&tv, nullptr);
            GlobalData::scene_time.insert(
                    std::pair<int, int>(scene_id, tv.tv_sec));

            // remove user 2
            GlobalData::user_match.erase(x.first);

            delete msg3;
            delete msg13;
            break;
        }
    }
}

template<>
ProtoMessageHandle<ProtoMsg::Match_Begin_Response>
        ProtoMessageHandle<ProtoMsg::Match_Begin_Response>::handle_ = {};

// match complete
template<>
void ProtoMessageHandle<ProtoMsg::Match_Complete_Request>
::Process(const ProtoMsg::Message &msg) {}

template<>
ProtoMessageHandle<ProtoMsg::Match_Complete_Request>
        ProtoMessageHandle<ProtoMsg::Match_Complete_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Match_Complete_Response>
::Process(const ProtoMsg::Message &msg) {}

template<>
ProtoMessageHandle<ProtoMsg::Match_Complete_Response>
        ProtoMessageHandle<ProtoMsg::Match_Complete_Response>::handle_ = {};

// match cancel
template<>
void ProtoMessageHandle<ProtoMsg::Match_Cancel_Request>
::Process(const ProtoMsg::Message &msg) {
    // remove user from user match data
    GlobalData::user_match.erase(msg.userid());

    auto msg1 = new ProtoMsg::MatchCancelResponse;
    msg1->set_id(1);
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_match_cancel(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_userid(msg.userid());
    msg3->set_msg_type(ProtoMsg::MSG::Match_Cancel_Response);
    msg3->set_allocated_response(msg2);

    std::shared_ptr<Worker> worker(new Worker(*msg3));
    GlobalData::write_message_queue.Add(worker);

    delete msg3;
}

template<>
ProtoMessageHandle<ProtoMsg::Match_Cancel_Request>
        ProtoMessageHandle<ProtoMsg::Match_Cancel_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Match_Cancel_Response>
::Process(const ProtoMsg::Message &msg) {
    auto *tcp = GlobalData::user_client.find(msg.userid())->second;
    auto message = MessagePacket::Pack(msg);
    tcp->write(message->data, message->length);
}

template<>
ProtoMessageHandle<ProtoMsg::Match_Cancel_Response>
        ProtoMessageHandle<ProtoMsg::Match_Cancel_Response>::handle_ = {};

// plane operate
template<>
void ProtoMessageHandle<ProtoMsg::Plane_Operate_Request>
::Process(const ProtoMsg::Message &msg) {
    auto msg1 = new ProtoMsg::PlaneOperateResponse;
    msg1->set_horizontal(msg.request().plane_operate().horizontal());
    msg1->set_vertical(msg.request().plane_operate().vertical());
    msg1->set_fire(msg.request().plane_operate().fire());
    msg1->set_user_id(msg.request().plane_operate().user_id());
    msg1->set_index_id(msg.request().plane_operate().index_id());
    msg1->set_x(msg.request().plane_operate().x());
    msg1->set_y(msg.request().plane_operate().y());
    msg1->set_z(msg.request().plane_operate().z());
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_plane_operate(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_sequence(msg.sequence());
    msg3->set_userid(msg.userid());
    msg3->set_timestamp_ms(Util::TimeStamp::GetTimeStampMs());
    msg3->set_msg_type(ProtoMsg::MSG::Plane_Operate_Response);
    msg3->set_allocated_response(msg2);

    std::shared_ptr<Worker> worker(new Worker(*msg3));
    GlobalData::write_message_queue.Add(worker);
    delete msg3;
}

template<>
ProtoMessageHandle<ProtoMsg::Plane_Operate_Request>
        ProtoMessageHandle<ProtoMsg::Plane_Operate_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Plane_Operate_Response>
::Process(const ProtoMsg::Message &msg) {
    auto message = MessagePacket::Pack(msg);
    if (GlobalData::user_scene.find(msg.userid()) != GlobalData::user_scene.end()) {
        int scene = GlobalData::user_scene.find(msg.userid())->second;
        if (GlobalData::scene_user.find(scene) != GlobalData::scene_user.end()) {
            GlobalData::SceneUsers scene_users = GlobalData::scene_user.find(scene)->second;
            if (scene_users.one != 0) {
                auto *tcp = GlobalData::user_client.at(scene_users.one);
                tcp->write(message->data, message->length);
            }
            if (scene_users.two != 0) {
                auto *tcp = GlobalData::user_client.at(scene_users.two);
                tcp->write(message->data, message->length);
            }
        }
    }
}

template<>
ProtoMessageHandle<ProtoMsg::Plane_Operate_Response>
        ProtoMessageHandle<ProtoMsg::Plane_Operate_Response>::handle_ = {};

// create waves
template<>
void ProtoMessageHandle<ProtoMsg::Wave_Create_Response>
::Process(const ProtoMsg::Message &msg) {
    auto message = MessagePacket::Pack(msg);
    auto *tcp = GlobalData::user_client.find(msg.userid())->second;
    tcp->write(message->data, message->length);
}

template<>
ProtoMessageHandle<ProtoMsg::Wave_Create_Response>
        ProtoMessageHandle<ProtoMsg::Wave_Create_Response>::handle_ = {};

// exit scene
template<>
void ProtoMessageHandle<ProtoMsg::Exit_Scene_Request>
::Process(const ProtoMsg::Message &msg) {
    // deal scene ref
    auto user_id = msg.userid();
    if (GlobalData::user_scene.find(user_id) !=
        GlobalData::user_scene.end()) {
        auto scene = GlobalData::user_scene.find(user_id)->second;
        if (GlobalData::scene_user.find(scene) !=
            GlobalData::scene_user.end()) {
            auto scene_user = &GlobalData::scene_user.find(scene)->second;
            if (scene_user->one == user_id) {
                scene_user->one = 0;
            }
            if (scene_user->two == user_id) {
                scene_user->two = 0;
            }
            if (scene_user->one == 0 && scene_user->two == 0) {
                GlobalData::scene_user.erase(scene);
                GlobalData::scene_time.erase(scene);
            }
        }
        GlobalData::user_scene.erase(user_id);
    }

    auto msg1 = new ProtoMsg::ExitSceneResponse;
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_exit_scene(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_userid(msg.userid());
    msg3->set_msg_type(ProtoMsg::MSG::Exit_Scene_Response);
    msg3->set_allocated_response(msg2);
    std::shared_ptr<Worker> worker(new Worker(*msg3));
    GlobalData::write_message_queue.Add(worker);
    delete msg3;
}

template<>
ProtoMessageHandle<ProtoMsg::Exit_Scene_Request>
        ProtoMessageHandle<ProtoMsg::Exit_Scene_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Exit_Scene_Response>
::Process(const ProtoMsg::Message &msg) {
    auto message = MessagePacket::Pack(msg);
    if (GlobalData::user_client.find(msg.userid()) != GlobalData::user_client.end()) {
        auto *tcp = GlobalData::user_client.find(msg.userid())->second;
        tcp->write(message->data, message->length);
    }
}

template<>
ProtoMessageHandle<ProtoMsg::Exit_Scene_Response>
        ProtoMessageHandle<ProtoMsg::Exit_Scene_Response>::handle_ = {};

// time sync
template<>
void ProtoMessageHandle<ProtoMsg::Time_Sync_Request>
::Process(const ProtoMsg::Message &msg) {
    LOG(INFO) << "Time_Sync_Request";
    auto msg1 = new ProtoMsg::TimeSyncResponse;
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_time_sync(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_userid(msg.userid());
    msg3->set_msg_type(ProtoMsg::MSG::Time_Sync_Response);
    msg3->set_timestamp_ms(Util::TimeStamp::GetTimeStampMs());
    msg3->set_allocated_response(msg2);

    auto message = MessagePacket::Pack(msg);
    if (GlobalData::user_client.find(msg.userid()) != GlobalData::user_client.end()) {
        auto *tcp = GlobalData::user_client.find(msg.userid())->second;
        tcp->write(message->data, message->length);
    }

    delete msg3;
}

template<>
ProtoMessageHandle<ProtoMsg::Time_Sync_Request>
        ProtoMessageHandle<ProtoMsg::Time_Sync_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Time_Sync_Response>
::Process(const ProtoMsg::Message &msg) {}

template<>
ProtoMessageHandle<ProtoMsg::Time_Sync_Response>
        ProtoMessageHandle<ProtoMsg::Time_Sync_Response>::handle_ = {};

// plane destroy
template<>
void ProtoMessageHandle<ProtoMsg::Plane_Destroy_Request>
::Process(const ProtoMsg::Message &msg) {
    auto msg1 = new ProtoMsg::PlaneDestroyResponse;
    msg1->set_index_id(msg.request().plane_destroy().index_id());
    auto msg2 = new ProtoMsg::Response;
    msg2->set_allocated_plane_destroy(msg1);
    auto msg3 = new ProtoMsg::Message;
    msg3->set_userid(msg.userid());
    msg3->set_msg_type(ProtoMsg::MSG::Plane_Destroy_Response);
    msg3->set_timestamp_ms(Util::TimeStamp::GetTimeStampMs());
    msg3->set_allocated_response(msg2);
    std::shared_ptr<Worker> worker(new Worker(*msg3));
    GlobalData::write_message_queue.Add(worker);
    delete msg3;
}

template<>
ProtoMessageHandle<ProtoMsg::Plane_Destroy_Request>
        ProtoMessageHandle<ProtoMsg::Plane_Destroy_Request>::handle_ = {};

template<>
void ProtoMessageHandle<ProtoMsg::Plane_Destroy_Response>
::Process(const ProtoMsg::Message &msg) {
    auto message = MessagePacket::Pack(msg);
    if (GlobalData::user_scene.find(msg.userid()) != GlobalData::user_scene.end()) {
        int scene = GlobalData::user_scene.find(msg.userid())->second;
        if (GlobalData::scene_user.find(scene) != GlobalData::scene_user.end()) {
            GlobalData::SceneUsers scene_users = GlobalData::scene_user.find(scene)->second;
            if (scene_users.one != 0) {
                auto *tcp = GlobalData::user_client.at(scene_users.one);
                tcp->write(message->data, message->length);
            }
            if (scene_users.two != 0) {
                auto *tcp = GlobalData::user_client.at(scene_users.two);
                tcp->write(message->data, message->length);
            }
        }
    }
}

template<>
ProtoMessageHandle<ProtoMsg::Plane_Destroy_Response>
        ProtoMessageHandle<ProtoMsg::Plane_Destroy_Response>::handle_ = {};