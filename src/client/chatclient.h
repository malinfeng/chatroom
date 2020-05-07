#pragma once

#include <string>

#include "../../lion/TcpConnection.h"
#include "../../build/src/chat.pb.h"
#include "../ChatMsg.h"

class ChatClient
{
public:
    ChatClient(std::string serverIp, int serverPort);
    ~ChatClient();

    void start();

private:
    

    void listWelcome();
    void listLogin();
    void listRegist();
    void ClientExit();

    void listMain();

    void listChatRoom();
    void listRecord();
    void listUser();
    
    void listSendPubMsg();
    void listSendPriMsg();

    int init();

    //接收线程函数
    static void* recvFun(void *arg);

    int sendMsgtoLocal(chatroom::ChatMsg& msg);
    int getMsgfromLocal(chatroom::ChatMsg& msg);
    int sendMsgtoServer(chatroom::ChatMsg& msg);
    int getMsgfromServer(chatroom::ChatMsg& msg);

    int cmdEnterChatRoom();
    int cmdLeaveChatRoom();
    int cmdWelcome();

private:  
    std::string _serverIp;
    int _port;
    int _sockFd;

    pthread_t _precvId;

    //本地套接字
    //_fd[0]用于接收线程写数据,_fd[1]用于发送线程读数据
    int _fd[2];

    //
    int _inChatRoom;

    bool _quit;

    unsigned int _uid;
};