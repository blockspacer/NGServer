#include <uv.h>
#include <cstdlib>
#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <bitset>

#include <uv.h>
#include <glog/logging.h>

#include "../uvw/uvw.hpp"

#include "../Proto/Message.pb.h"

#include "StringConversion.h"
#include "UUID.h"
#include "ProtoBaseHandle.h"
#include "DatabaseInstance.h"
#include "MessageQueue.h"
#include "StringConversion.h"

#include "Global.h"

//const auto DEFAULT_IP = "127.0.0.1"; // local
//const auto DEFAULT_IP = "192.168.62.137"; // home
//const auto DEFAULT_IP = "192.168.107.128"; // work
//const auto DEFAULT_IP = "111.231.98.252"; // tengxunyun
const auto DEFAULT_IP = "0.0.0.0";
const auto DEFAULT_PORT = 7000;
const auto DEFAULT_BACKLOG = 128;

void ClientDisconnect(uvw::TcpHandle &tcp) {
    // remove auth user map
    auto it = GlobalData::client_user.find(&tcp);
    if (it != GlobalData::client_user.end()) {
        auto user_id = it->second;

        // deal fd ref
        int fd = (int) tcp.fileno();
        GlobalData::fd_tcp.erase(fd);

        // deal user ref
        GlobalData::client_user.erase(it);
        GlobalData::user_client.erase(user_id);
        GlobalData::user_auth.erase(user_id);
        GlobalData::user_auth.erase((int) tcp.fileno());

        // deal match ref
        if (GlobalData::user_match.find(user_id)
            != GlobalData::user_match.end()) {
            GlobalData::user_match.erase(user_id);
        }

        // deal scene ref
        if (GlobalData::user_scene.find(user_id)
            != GlobalData::user_scene.end()) {
            auto scene = GlobalData::user_scene.find(user_id)->second;
            if (GlobalData::scene_user.find(scene) !=
                GlobalData::scene_user.end()) {
                auto scene_user = &GlobalData::scene_user.find(
                        scene)->second;
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
    }

    int count = 0;
    tcp.loop().walk([&count](uvw::BaseHandle &handle) {
        if (handle.type() == uvw::HandleType::TCP) {
            LOG(INFO) << "uvw::HandleType::TCP";
        }
        ++count;
    });

    LOG(INFO) << "current client count: " << GlobalData::client_user.size()
              << " still alive: " << count << " handles";

    // close client tcp handle
    tcp.close();
}

/**
 * @brief start server, wait for client connect
 */
void ListenServer(uvw::Loop &loop) {
    LOG(INFO) << "ListenServer, IP: " << DEFAULT_IP
              << " Port: " << DEFAULT_PORT;
    std::shared_ptr<uvw::TcpHandle> tcp = loop.resource<uvw::TcpHandle>();

    uvw::TcpHandle::Time ttime = std::chrono::duration<int>(5);
    LOG(INFO) << "KeepAlive: " << tcp->keepAlive(true, ttime);

    tcp->on<uvw::ListenEvent>([](const uvw::ListenEvent &,
                                 uvw::TcpHandle &srv) {
        std::shared_ptr<uvw::TcpHandle> client =
                srv.loop().resource<uvw::TcpHandle>();

        client->on<uvw::ErrorEvent>([ptr = srv.shared_from_this()]
                                            (const uvw::ErrorEvent &,
                                             uvw::TcpHandle &client) {
            LOG(INFO) << "client uvw::ErrorEvent";
            ClientDisconnect(client);
            ptr->close();
        });


        client->on<uvw::CloseEvent>([ptr = srv.shared_from_this()]
                                            (const uvw::CloseEvent &,
                                             uvw::TcpHandle &) {
            LOG(INFO) << "client uvw::CloseEvent";
            // ptr->close();
        });

        // read data from client
        client->on<uvw::DataEvent>([](const uvw::DataEvent &event,
                                      uvw::TcpHandle &tcp) {
            std::string read_data(event.data.get(), event.length);
            read_data = read_data.substr(4, read_data.length() - 4);

            int fd = (int) tcp.fileno();
            if (GlobalData::fd_tcp.find(fd) ==
                GlobalData::fd_tcp.end()) {
                GlobalData::fd_tcp.insert(
                        std::pair<int, uvw::TcpHandle *>(fd, &tcp));
            }

            // message queue
            auto *proto_msg = new ProtoMsg::Message;
            proto_msg->ParseFromString(read_data);
            proto_msg->set_session_id(fd);


            std::shared_ptr<Worker> worker(new Worker(*proto_msg));
            GlobalData::read_message_queue.Add(worker);

            delete proto_msg;
        });

        client->on<uvw::WriteEvent>([](const uvw::WriteEvent &event,
                                       uvw::TcpHandle &tcp) {});

        // client tcp disconnect
        client->on<uvw::EndEvent>([ptr = srv.shared_from_this()]
                                          (const uvw::EndEvent &,
                                           uvw::TcpHandle &client_handle) {
            LOG(INFO) << "client uvw::EndEvent";
            ClientDisconnect(client_handle);
        });

        srv.accept(*client);

        uvw::Addr local = srv.sock();
        DLOG(INFO) << "local: " << local.ip << " " << local.port;
        uvw::Addr remote = client->peer();
        DLOG(INFO) << "remote: " << remote.ip << " " << remote.port;

        client->read();
    });

    tcp->on<uvw::CloseEvent>([](const uvw::CloseEvent &,
                                uvw::TcpHandle &tcp) {
        LOG(INFO) << "server uvw::CloseEvent";
    });

    tcp->on<uvw::ConnectEvent>([](const uvw::ConnectEvent &event,
                                  uvw::TcpHandle &tcp) {
        LOG(INFO) << "server uvw::ConnectEvent";
    });

    tcp->on<uvw::ShutdownEvent>([](const uvw::ShutdownEvent &event,
                                   uvw::TcpHandle &tcp) {
        LOG(INFO) << "server uvw::ShutdownEvent";
    });

    tcp->bind(DEFAULT_IP, DEFAULT_PORT);
    tcp->listen();
}

/**
 * @brief send message to client 20 times every second
 *
 * timer run consumer.Run()
 */
void NotifyClient(uvw::Loop &loop) {
    DLOG(INFO) << "TimerHanlde 50ms NotifyClient";
    std::shared_ptr<uvw::TimerHandle> handle =
            loop.resource<uvw::TimerHandle>();
    handle->on<uvw::ErrorEvent>([](const auto &, auto &) {
        DLOG(ERROR) << "NotifyClient uvw::ErrorEvent";
    });
    handle->on<uvw::TimerEvent>([](const auto &, auto &) {
        GlobalData::read_consumer.RunRead();   // deal with read data
        GlobalData::write_consumer.RunWrite();  // write data to client
        GlobalData::WavesCreate();
    });
    handle->start(uvw::TimerHandle::Time{0}, uvw::TimerHandle::Time{50});
}

void BackWorker(uvw::Loop &loop) {
    bool checkErrorEvent = false;
    // do work thing here, nonblcok other thread
    std::shared_ptr<uvw::WorkReq> req = loop.resource<uvw::WorkReq>([]() {
        LOG(INFO) << "BackWorker uvw::WorkReq tid: " << uv_thread_self();
        sleep(10);
    });
    // work complete callback
    req->on<uvw::WorkEvent>([](const auto &, auto &) {
        LOG(INFO) << "BackWorker uvw::WorkEvent";
    });
    req->on<uvw::ErrorEvent>([&checkErrorEvent](const auto &, auto &) {
        LOG(ERROR) << "BackWorker uvw::ErrorEvent";
        checkErrorEvent = true;
    });
    req->queue();
}

int main(int argc, char *argv[]) {

    // glog init
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    LOG(INFO) << "server start, pid: " << getpid()
              << ", tid: " << pthread_self();

    // connect mysql
    DBInstance::conn.Connect();
    auto db = DBInstance::instance();
    LOG(INFO) << "connect database "
              << (db.conn.IsConnected() ? "success" : "failed");

    // libuv tcp init
    auto loop = uvw::Loop::getDefault();

    ListenServer(*loop);
    NotifyClient(*loop);
    // BackWorker(*loop);

    auto a = ProtoMsg::Message();
    std::ifstream is("Test.txt");
    a.ParseFromIstream(&is);
//    LOG(INFO) << a.DebugString();

    // libuv run
    loop->run<uvw::Loop::Mode::DEFAULT>();
    loop = nullptr;

    // close glog
    google::ShutdownGoogleLogging();

    return 0;
}
