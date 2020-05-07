#include "chatui.h"


chatUi* chatUi::_sglton = NULL;

chatUi::chatUi()
{
    _chatDisplayStart = 0;
    _chatRoomMsg.resize(CHAT_ROOM_MSG_MAX);
}

chatUi::~chatUi()
{

}

void chatUi::displayCon()
{
    system("clear");
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "                小峰的聊天室                " << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "                  正在连接....              " << std::endl;
}

int chatUi::displayWelcome()
{    
    while (1)
    {
        setbuf(stdin, NULL);
        system("clear");
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "              小峰的聊天室                 " << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "            连接成功!请选择：              " << std::endl;
        std::cout << "              1.登陆                      " << std::endl;
        std::cout << "              2.注册                      " << std::endl;
        std::cout << "              3.退出                      " << std::endl;
        for (int i = 0; i < CHAT_ROOM_MSG_MAX - 3; ++i)
        {
            std::cout << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;

        int choose;
        std::cin >> choose;
        switch (choose)
        {
        case 1 :
        case 2 :
        case 3 :
            return choose;

        default:
            std::cout << "无效选择！" << std::endl;
            getchar();
            continue;
        }
    }

}

std::vector<std::string> chatUi::displayLogin()
{
    std::vector<std::string> ret;
    std::string uid;
    std::string password;

    setbuf(stdin, NULL);
    system("clear");
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "              小峰的聊天室                 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "               请输入账号：                " << std::endl;
    for (int i = 0; i < CHAT_ROOM_MSG_MAX ; ++i)
    {
        std::cout << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

    std::cin >> uid;

    system("clear");
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "              小峰的聊天室                 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "               请输入密码：                " << std::endl;
    for (int i = 0; i < CHAT_ROOM_MSG_MAX; ++i)
    {
        std::cout << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
    std::cin >> password;

    ret.push_back(uid);
    ret.push_back(password);
    
    return ret;
}

std::string chatUi::displayRegist(std::string uid)
{
    system("clear");
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "              小峰的聊天室                 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "         注册成功!   您的账号是：           " << std::endl;
    std::cout << uid << std::endl;
    std::cout << "            请输入您的密码：       " << std::endl;
    for (int i = 0; i < CHAT_ROOM_MSG_MAX - 2; ++i)
    {
        std::cout << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

    std::string password;
    std::cin >> password;
    return password;
}

int chatUi::displayMain()
{
    while (1)
    {
        setbuf(stdin, NULL);
        system("clear");
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "              小峰的聊天室                 " << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "                请选择：                  " << std::endl;
        std::cout << "                1.进入聊天室               " << std::endl;
        std::cout << "                2.查看在线用户             " << std::endl;
        std::cout << "                3.查看聊天记录             " << std::endl;
        std::cout << "                4.退出                    " << std::endl;
        for (int i = 0; i < CHAT_ROOM_MSG_MAX - 4; ++i)
        {
            std::cout << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;

        int choose;
        std::cin >> choose;
        switch (choose)
        {
        case 1 :
        case 2 :
        case 3 :
        case 4 :
            return choose;

        default:
            std::cout << "无效选择！" << std::endl;
            getchar();
            continue;
        }
    }
}

void chatUi::displayChatroom()
{
    system("clear");
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "              小峰的聊天室                 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "    请选择：1.群聊  2.私聊  3.退出聊天室     " << std::endl;
    int stridx = _chatDisplayStart;
    for (int i = 0; i < CHAT_ROOM_MSG_MAX; ++i)
    {
        if (stridx == CHAT_ROOM_MSG_MAX)
        {
            stridx = 0;
        }
        std::cout << _chatRoomMsg[stridx] << std::endl;
        stridx++;
    }
    std::cout << "------------------------------------------" << std::endl;
}


void chatUi::displayRecord()
{
    
}

std::string chatUi::displaySendPubMsg()
{
    setbuf(stdin, NULL);
    std::cout << "请输入您要发送的消息：" << std::endl;
    std::string msg;
    std::cin >> msg;
    return msg;
}

std::vector<std::string> chatUi::displaySendPriMsg()
{
    setbuf(stdin, NULL);
    std::cout << "请输入您要发送的用户id：" << std::endl;
    std::string id;
    std::cin >> id;
    std::cout << "请输入您要发送的消息：" << std::endl;
    std::string msg;
    std::cin >> msg;
    std::vector<std::string> ret;
    ret.push_back(id);
    ret.push_back(msg);

    return ret;
}

void chatUi::chatRoomAddMsg(std::string msg)
{
    _chatRoomMsg[_chatDisplayStart] = msg;
    _chatDisplayStart++;
    if (_chatDisplayStart == CHAT_ROOM_MSG_MAX)
    {
        _chatDisplayStart = 0;
    }
}