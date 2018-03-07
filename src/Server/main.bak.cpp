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

#include <uv.h>
#include <glog/logging.h>

#include "../Proto/Message.pb.h"

#include "Message.h"
#include "StringConversion.h"
#include "UvTcp.h"
#include "UvTimer.h"
#include "UUID.h"
#include "Account.h"
#include "TimerObserver.h"
#include "ProtoBaseHandle.h"
#include "DatabaseInstance.h"
#include "MessageQueue.h"

#include "uvw.hpp"

const auto DEFAULT_IP = "127.0.0.1";
const auto DEFAULT_PORT = 7000;
const auto DEFAULT_BACKLOG = 128;

std::vector<std::shared_ptr<uvw::Thread>> thread_list = {0, 0, 0};

void Listen(uvw::Loop &loop) {
    std::shared_ptr<uvw::TcpHandle> tcp = loop.resource<uvw::TcpHandle>();

    tcp->on<uvw::ListenEvent>([](const uvw::ListenEvent &,
                                 uvw::TcpHandle &srv) {
        LOG(INFO) << "server uvw::ListenEvent";

        std::shared_ptr<uvw::TcpHandle> client = srv.loop().resource<uvw::TcpHandle>();

        client->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &,
                                       uvw::TcpHandle &) {
            LOG(ERROR) << "client uvw::ErrorEvent";
        });

        client->on<uvw::CloseEvent>([ptr = srv.shared_from_this()](const uvw::CloseEvent &,
                                                                   uvw::TcpHandle &) {
            LOG(INFO) << "client uvw::CloseEvent";
            ptr->close();
        });

        client->on<uvw::DataEvent>([](const uvw::DataEvent &event, uvw::TcpHandle &tcp) {
            LOG(INFO) << "client uvw::DataEvent";
            LOG(INFO).write(event.data.get(), event.length);
            LOG(INFO) << "data length: " << event.length;
        });

        client->on<uvw::WriteEvent>([](const uvw::WriteEvent &event, uvw::TcpHandle &tcp) {
            LOG(INFO) << "client uvw::WriteEvent";
        });

        client->on<uvw::EndEvent>([](const uvw::EndEvent &, uvw::TcpHandle &handle) {
            LOG(INFO) << "client uvw::EndEvent";
            int count = 0;
            handle.loop().walk([&count](uvw::BaseHandle &) {
                ++count;
            });
            LOG(INFO) << "still alive: " << count << " handles";
            // handle.close();
        });

        srv.accept(*client);

        uvw::Addr local = srv.sock();
        LOG(INFO) << "local: " << local.ip << " " << local.port;

        uvw::Addr remote = client->peer();
        LOG(INFO) << "remote: " << remote.ip << " " << remote.port;

        client->read();
    });

    tcp->on<uvw::CloseEvent>([](const uvw::CloseEvent &, uvw::TcpHandle &) {
        LOG(INFO) << "server uvw::CloseEvent";
    });

    tcp->on<uvw::ConnectEvent>([](const uvw::ConnectEvent &event, uvw::TcpHandle &tcp) {
        LOG(INFO) << "server uvw::ConnectEvent";
    });

    tcp->on<uvw::ShutdownEvent>([](const uvw::ShutdownEvent &event, uvw::TcpHandle &tcp) {
        LOG(INFO) << "server uvw::ShutdownEvent";
    });

    tcp->bind(DEFAULT_IP, DEFAULT_PORT);
    tcp->listen();
}

void Timer1(uvw::Loop &loop) {
    LOG(INFO) << "Timer1 1000 1000";
    std::shared_ptr<uvw::TimerHandle> handle = loop.resource<uvw::TimerHandle>();
    handle->on<uvw::ErrorEvent>([](const auto &, auto &) {
        LOG(ERROR) << "Timer1 uvw::ErrorEvent";
    });
    handle->on<uvw::TimerEvent>([](const auto &, auto &) {
        LOG(INFO) << "Timer1 uvw::TimerEvent";
    });
    handle->start(uvw::TimerHandle::Time{1000}, uvw::TimerHandle::Time{1000});
}

void Idle(uvw::Loop &loop) {
    std::shared_ptr<uvw::IdleHandle> handle = loop.resource<uvw::IdleHandle>();
    handle->on<uvw::ErrorEvent>([](const auto &, auto &) {
        LOG(ERROR) << "error";
    });
    handle->on<uvw::IdleEvent>([](const auto &, auto &) {
        // LOG(INFO) << "Idle";
    });
    handle->start();
}

void TimerWriter(uvw::Loop &loop) {
    std::shared_ptr<uvw::TimerHandle> handle = loop.resource<uvw::TimerHandle>();
    handle->on<uvw::ErrorEvent>([](const auto &, auto &) {
        LOG(ERROR) << "error";
    });
    handle->on<uvw::TimerEvent>([](const auto &, auto &) {
        LOG(INFO) << "TimerWriter";
    });
    handle->start(uvw::TimerHandle::Time{1000}, uvw::TimerHandle::Time{3000});
}

void Worker1(uvw::Loop &loop) {
    bool checkErrorEvent = false;
    std::shared_ptr<uvw::WorkReq> req = loop.resource<uvw::WorkReq>([]() {
        LOG(INFO) << "Worker1 uvw::WorkReq tid: " << uv_thread_self();
    });
    req->on<uvw::WorkEvent>([](const auto &, auto &) { // work complete callback
        LOG(INFO) << "Worker1 uvw::WorkEvent";
    });
    req->on<uvw::ErrorEvent>([&checkErrorEvent](const auto &, auto &) {
        LOG(INFO) << "Worker1 uvw::ErrorEvent";
        checkErrorEvent = true;
    });
    req->queue();
}

void Worker2(uvw::Loop &loop) {
    bool checkErrorEvent = false;
    std::shared_ptr<uvw::WorkReq> req = loop.resource<uvw::WorkReq>([]() {
        LOG(INFO) << "Worker2 uvw::WorkReq tid: " << uv_thread_self();
    });
    req->on<uvw::WorkEvent>([](const auto &, auto &) {
        LOG(INFO) << "Worker2 uvw::WorkEvent";
    });
    req->on<uvw::ErrorEvent>([&checkErrorEvent](const auto &, auto &) {
        LOG(INFO) << "Worker2 uvw::ErrorEvent";
        checkErrorEvent = true;
    });
    req->queue();
}

void Thread1(uvw::Loop &loop) {
    int in = 1;
    std::shared_ptr<void> data = std::make_shared<int>(in);
//    std::shared_ptr<uvw::Thread> thr = loop.resource<uvw::Thread>([](std::shared_ptr<void> tmp) {
    thread_list[0] = loop.resource<uvw::Thread>([](std::shared_ptr<void> tmp) {
        LOG(INFO) << "Thread1 uvw::Thread tid: " << uv_thread_self();
        int i = 0;
        while (i < 1000000000) {
            ++i;
        }
        if (tmp == nullptr) {
            LOG(INFO) << "Thread1 data is nullptr";
        }
        std::shared_ptr<int> a = std::static_pointer_cast<int>(tmp);
        LOG(INFO) << "Thread1 sleep 1 get int " << *a;
    }, data);

    thread_list[0]->run();
    // thread_list[0]->join();
}

void Thread2(uvw::Loop &loop) {
    int in = 2;
    std::shared_ptr<void> data = std::make_shared<int>(in);
//    std::shared_ptr<uvw::Thread> thr = loop.resource<uvw::Thread>([](std::shared_ptr<void> tmp) {
    thread_list[1] = loop.resource<uvw::Thread>([](std::shared_ptr<void> tmp) {
        LOG(INFO) << "Thread2 uvw::Thread tid: " << uv_thread_self();
        sleep(2);
        if (tmp == nullptr) {
            LOG(INFO) << "Thread2 data is nullptr";
        }
        std::shared_ptr<int> a = std::static_pointer_cast<int>(tmp);
        LOG(INFO) << "Thread2 sleep 2 get int " << *a;
    }, data);

    thread_list[1]->run();
    // thread_list[1]->join();
}

void Thread3(uvw::Loop &loop) {
    int in = 3;
    std::shared_ptr<void> data = std::make_shared<int>(in);
    thread_list[2] = loop.resource<uvw::Thread>([](std::shared_ptr<void> tmp) {
        LOG(INFO) << "Thread3 uvw::Thread tid: " << uv_thread_self();
        sleep(3);
        if (tmp == nullptr) {
            LOG(INFO) << "Thread3 data is nullptr";
        }
        std::shared_ptr<int> a = std::static_pointer_cast<int>(tmp);
        LOG(INFO) << "Thread3 sleep 3 get int " << *a;
    }, data);

    thread_list[2]->run();
    // thread_list[2]->join();
}

void Async1(uvw::Loop &loop) {
    std::shared_ptr<uvw::AsyncHandle> handle = loop.resource<uvw::AsyncHandle>();

    handle->on<uvw::AsyncEvent>([](const auto &, auto &) {
        LOG(INFO) << "Async1 uvw::AsyncEvent tid: " << uv_thread_self();
    });

    handle->on<uvw::ErrorEvent>([](const auto &, auto &) {
        LOG(INFO) << "Async1 uvw::ErrorEvent tid: " << uv_thread_self();
    });

    handle->send();
    handle->close();

    LOG(INFO) << "Async1 active: " << handle->active();
    LOG(INFO) << "Async1 closing: " << handle->closing();
}

void ThreadOnce(uvw::Loop &loop) {
    std::shared_ptr<uvw::Once> req = loop.resource<uvw::Once>();
    req->once([]() {
        LOG(INFO) << "ThreadOnce";
    });
}

void ThreadCallback(void *arg) {
    LOG(INFO) << "ThreadCallback tid: " << uv_thread_self();
//     int *in = (int *) arg;
    sleep(3);
    std::string *in = (std::string *) arg;
    LOG(INFO) << "Thread callback sleep 5 get string " << *in;
}


int main(int argc, char *argv[]) {

    // glog init
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    LOG(INFO) << "server start, pid: " << getpid() << ", tid: " << pthread_self();

    // libuv tcp init
    auto loop = uvw::Loop::getDefault();

    // libuv run
    loop->run<uvw::Loop::Mode::DEFAULT>();
    loop = nullptr;

    // close glog
    google::ShutdownGoogleLogging();

    return 0;
}

// protobuf test message

// auto *msg1 = new ProtoMsg::RegisterResquest;
// msg1->set_name("cwj");
// msg1->set_password("123");
// msg1->set_email("user@example.com");
// auto *msg2 = new ProtoMsg::Request;
// msg2->set_allocated_register_(msg1);
// auto *msg3 = new ProtoMsg::Message;
// msg3->set_msg_type(ProtoMsg::MSG::Register_Request);
// msg3->set_sequence(10);
// msg3->set_session_id(20);
// msg3->set_allocated_request(msg2);