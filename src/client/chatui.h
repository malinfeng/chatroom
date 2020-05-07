#pragma once

#include <iostream>
#include <string>
#include <vector>

#define CHAT_ROOM_MSG_MAX 10

class chatUi
{
    public:

    static chatUi* getInstance()
    {
        if (_sglton == NULL)
        {
            _sglton = new chatUi;
        }
        return _sglton;
    }

    //连接界面
    void displayCon();

    //欢迎界面,返回用户选择
    int displayWelcome();

    //登陆界面，返回用户输入的用户名与密码
    std::vector<std::string> displayLogin();

    //注册界面,输出给定的注册ID,返回用户设定的密码
    std::string displayRegist(std::string uid);

    //主界面,返回用户选择
    int displayMain();

    //聊天室界面,刷新聊天内容的显示
    void displayChatroom();

    //聊天记录界面
    void displayRecord();

    //聊天室增加消息
    void chatRoomAddMsg(std::string msg);

    //群聊消息
    std::string displaySendPubMsg();

    //私聊消息
    std::vector<std::string> displaySendPriMsg();

    private:
        chatUi();
        ~chatUi();

    //
    private:
    static chatUi* _sglton;

    
    std::vector<std::string> _chatRoomMsg;
    int _chatDisplayStart;
};