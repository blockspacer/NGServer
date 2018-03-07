#include <unistd.h>

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <fstream>

#include "Message.pb.h"

TEST(ProtobufTest, MessageTest100000) {
    int i = 0;
    while (i < 100000) {
        auto *login_req = new ProtoMsg::LoginRequest;
        login_req->set_name("cwj");
        login_req->set_password("a123");

        auto *req = new ProtoMsg::Request;
        req->set_allocated_login(login_req);

        auto *msg = new ProtoMsg::Message;
        msg->set_msg_type(ProtoMsg::MSG::Login_Request);
        msg->set_sequence(10);
        msg->set_session_id(20);
        msg->set_allocated_request(req);

        if (i == 0) {
            std::string str;
            msg->SerializeToString(&str);

            auto *temp = new ProtoMsg::Message;
            temp->ParseFromString(str);

            ASSERT_EQ(msg->DebugString(), temp->DebugString());
        }

        ++i;
        delete msg;
    }
}


TEST(ProtobufTest, MessageTest) {
    auto *login_req = new ProtoMsg::LoginRequest;
    login_req->set_name("cwj");
    login_req->set_password("123");

    auto *req = new ProtoMsg::Request;
    req->set_allocated_login(login_req);

    auto *msg = new ProtoMsg::Message;
    msg->set_msg_type(ProtoMsg::MSG::Login_Request);
    msg->set_sequence(10);
    msg->set_session_id(20);
    msg->set_allocated_request(req);

    std::string str;
    msg->SerializeToString(&str);

    auto *temp = new ProtoMsg::Message;
    temp->ParseFromString(str);

    ASSERT_EQ(msg->DebugString(), temp->DebugString());

    delete msg;
}



TEST(ProtobufTest, MessageTestCube) {
    auto *msg1 = new ProtoMsg::CubeOperateRequest;
    msg1->set_movement(0.1);
    msg1->set_turn(0.3);
    msg1->set_id(15);

    auto *msg2 = new ProtoMsg::Request;
    msg2->set_allocated_cube_operate(msg1);

    auto *msg = new ProtoMsg::Message;
    msg->set_msg_type(ProtoMsg::MSG::Cube_Operate_Request);
    msg->set_sequence(10);
    msg->set_session_id(20);
    msg->set_allocated_request(msg2);

    std::string str;
    msg->SerializeToString(&str);

    auto *temp = new ProtoMsg::Message;
    temp->ParseFromString(str);

    ASSERT_EQ(msg->DebugString(), temp->DebugString());

    std::ofstream out("output");
    out << str;
    out.close();

    LOG(INFO) << temp->DebugString();

    delete msg;
}