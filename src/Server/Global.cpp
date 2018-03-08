#include "Global.h"

std::map<int, uvw::TcpHandle *> GlobalData::fd_tcp = {};

std::map<uvw::TcpHandle *, int> GlobalData::client_user = {};

std::map<int, uvw::TcpHandle *> GlobalData::user_client = {};

std::map<int, bool> GlobalData::user_auth = {};

MessageQueue<std::shared_ptr<Worker>> GlobalData::read_message_queue =
        MessageQueue<std::shared_ptr<Worker>>();
Consumer GlobalData::read_consumer = Consumer(read_message_queue);

MessageQueue<std::shared_ptr<Worker>> GlobalData::write_message_queue =
        MessageQueue<std::shared_ptr<Worker>>();
Consumer GlobalData::write_consumer = Consumer(write_message_queue);

std::map<int, int> GlobalData::user_match = {};

std::map<int, int> GlobalData::user_scene = {};
std::map<int, GlobalData::SceneUsers> GlobalData::scene_user = {};
int GlobalData::scene_id = 0;

std::map<int, int> GlobalData::scene_time = {};

std::map<int, std::map<int, ProtoMsg::Message>> GlobalData::scene_data = {};

void GlobalData::WavesCreate() {
    struct timeval tv = {};
    gettimeofday(&tv, nullptr);
    for (auto &x : GlobalData::scene_time) {
        if (x.second < tv.tv_sec) {
            x.second = (int) tv.tv_sec + 0; // one second
            int user1 = GlobalData::scene_user.find(x.first)->second.one;
            int user2 = GlobalData::scene_user.find(x.first)->second.two;

            float pos_x = random() % 12 - 6;
            float type = random() % 2;

            if (user1 != 0) {
                auto msg1 = new ProtoMsg::WaveCreateResponse;
                msg1->set_x(pos_x);
                msg1->set_y(0);
                msg1->set_z(16);
                msg1->set_angel(0);
                msg1->set_type(type);
                auto msg2 = new ProtoMsg::Response;
                msg2->set_allocated_wave_create(msg1);
                auto msg3 = new ProtoMsg::Message;
                msg3->set_msg_type(ProtoMsg::MSG::Wave_Create_Response);
                msg3->set_allocated_response(msg2);
                msg3->set_userid(user1);
                std::string data1("");
                std::shared_ptr<Worker> worker1(new Worker(*msg3));
                GlobalData::write_message_queue.Add(worker1);
            }

            if (user2 != 0) {
                auto msg11 = new ProtoMsg::WaveCreateResponse;
                msg11->set_x(pos_x);
                msg11->set_y(0);
                msg11->set_z(16);
                msg11->set_angel(0);
                msg11->set_type(type);
                auto msg12 = new ProtoMsg::Response;
                msg12->set_allocated_wave_create(msg11);
                auto msg13 = new ProtoMsg::Message;
                msg13->set_msg_type(ProtoMsg::MSG::Wave_Create_Response);
                msg13->set_allocated_response(msg12);
                msg13->set_userid(user2);
                std::string data2("");
                msg13->SerializeToString(&data2);
                std::shared_ptr<Worker> worker2(new Worker(*msg13));
                GlobalData::write_message_queue.Add(worker2);
            }

        }
    }
}

