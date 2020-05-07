#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>
#include <unistd.h>
#include <pthread.h>

#include "chatclient.h"
#include "chatui.h"
#include "../../lion/Buffer.h"

ChatClient::ChatClient(std::string serverIp, int serverPort)
    :
    _serverIp(serverIp),
    _port(serverPort)
{
    _inChatRoom = 0;
    _sockFd = 0;
    _quit = false;
}

ChatClient::~ChatClient()
{

}

int ChatClient::init()
{
    if ( socketpair(AF_UNIX, SOCK_STREAM, 0, _fd) < 0)
    {
        return -1;
    }

    if( (_sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }

    int op = 1;
    if( setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(int)) == -1 )
    {
        return -1;
    }
        
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET ;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = inet_addr(_serverIp.c_str());

    if( connect( _sockFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        return -1;
    }

    if ( pthread_create(&_precvId, NULL, &ChatClient::recvFun, (void*)this) < 0)
    {
        return -1;
    }
    return 0;
}

void* ChatClient::recvFun(void *arg)
{
    ChatClient* curCli = (ChatClient *)arg;

    while (!curCli->_quit)
    {
        chatroom::ChatMsg msg;
        while(curCli->getMsgfromServer(msg) < 0)
        {
            
        }

        chat::ChatMsgHead head = msg.getHead();

        switch (head.type())
        {
        case chat::msg_type::chat_broadcast :
        {
            chat::ChatPublic cmd;
            cmd.ParseFromString(msg.getBodyStr());
            std::string msgstr = cmd.content();
            if (msgstr.size() != cmd.content_len())
            {
                break;
            }
            std::string infostr;
            infostr = std::to_string(cmd.from_id()) + std::string(" 对所有人说：   ") + msgstr;
            chatUi::getInstance()->chatRoomAddMsg(infostr);

            if (curCli->_inChatRoom)
            {
                chatUi::getInstance()->displayChatroom();
            }
            break;
        }
        case chat::msg_type::chat_private :
        {
            chat::ChatPrivate cmd;
            cmd.ParseFromString(msg.getBodyStr());
            std::string msgstr = cmd.content();
            if (msgstr.size() != cmd.content_len())
            {
                break;
            }
            std::string infostr;
            infostr = std::to_string(cmd.from_id()) + std::string(" 对你私聊说：   ") + msgstr;
            chatUi::getInstance()->chatRoomAddMsg(infostr);

            if (curCli->_inChatRoom)
            {
                chatUi::getInstance()->displayChatroom();
            }
            break;
        }
        default:
            curCli->sendMsgtoLocal(msg);

            break;
        }
    }

    return NULL;
}



void ChatClient::start()
{
    if( init() < 0)
    {
        return;
    }

    chatUi::getInstance()->displayCon();
    
    if ( cmdWelcome() < 0)
    {
        std::cout << "连接服务器失败" << std::endl;
        return ;
    }

    listWelcome();
}

void ChatClient::listWelcome()
{
    while (!_quit)
    {
        int welcomeChoose = chatUi::getInstance()->displayWelcome();

        switch ( welcomeChoose )
        {
        case 1://登陆
            listLogin();
            break;

        case 2://注册
            listRegist();
            break;

        case 3://退出程序
            ClientExit();
            break;
            
        default:
            break;
        }
    }
}

void ChatClient::listLogin()
{
    std::vector<std::string> user = chatUi::getInstance()->displayLogin();
    if (user.size() != 2)
    {
        return;
    }

    _uid = atoi(user[0].c_str());
    //用户登陆逻辑
    chat::Login cmd;
    cmd.set_uid(_uid);
    cmd.set_password(user[1]);

    chat::ChatMsgHead head;
    head.set_type(chat::msg_type::login);
    head.set_req(chat::msg_req::request);

    chatroom::ChatMsg msg;
    msg.setHead(head);
    msg.setMsg(&cmd);

    if (sendMsgtoServer(msg) < 0)
    {
        return;
    }

    chatroom::ChatMsg msgr;
    if (getMsgfromLocal(msgr) < 0)
    {
        return;
    }
    chat::Login cmdr;
    if (!cmdr.ParseFromString(msgr.getBodyStr()))
    {
        return;
    }
    
    if (cmdr.status() == chat::ret_status::failed)
    {
        return;
    }

    listMain();
}

void ChatClient::listMain()
{
    while (!_quit)
    {
        int mainChoose = chatUi::getInstance()->displayMain();
        
        switch ( mainChoose )
        {
        case 1://聊天室
        {
            cmdEnterChatRoom();
            listChatRoom();
            break;
        }
        case 2://在线用户
            listUser();
            break;

        case 3://聊天记录
            listRecord();
            break;

        case 4://退出
            ClientExit();
            break;

        default:
            break;
        }

        _inChatRoom = 0;
    }
    
}

void ChatClient::listRegist()
{

}

void ChatClient::ClientExit()
{
    _quit = true;
    pthread_cancel(_precvId);
    pthread_join(_precvId, NULL);
    exit(0);
}

void ChatClient::listChatRoom()
{
    _inChatRoom = 1;

    chatUi::getInstance()->displayChatroom();

    int choose = 0;
    while (choose != 3)
    {
        std::cin >> choose;
        switch (choose)
        {
        case 1://群聊
            listSendPubMsg();
            chatUi::getInstance()->displayChatroom();//刷新显示
            break;

        case 2://私聊
            listSendPriMsg();
            chatUi::getInstance()->displayChatroom();//刷新显示
            break;

        case 3://退出聊天室
        {
            cmdLeaveChatRoom();
            break;
        }
           
            
        default:
            break;
        }
    }
    
}

void ChatClient::listSendPubMsg()
{
    _inChatRoom = 0;
    std::string strmsg = chatUi::getInstance()->displaySendPubMsg();
    _inChatRoom = 1;

    chat::ChatPublic cmd;
    cmd.set_from_id(_uid);
    cmd.set_content(strmsg);
    cmd.set_content_len(strmsg.size());

    chat::ChatMsgHead head;
    head.set_type(chat::msg_type::chat_broadcast);
    head.set_req(chat::msg_req::request);

    chatroom::ChatMsg msg;
    msg.setHead(head);
    msg.setMsg(&cmd);

    if (sendMsgtoServer(msg) < 0)
    {
        std::cout << "发送消息失败" << std::endl;
    }
}

void ChatClient::listSendPriMsg()
{
    _inChatRoom = 0;
    std::vector<std::string> strmsg = chatUi::getInstance()->displaySendPriMsg();
    _inChatRoom = 1;

    chat::ChatPrivate cmd;
    cmd.set_from_id(_uid);
    unsigned int toid = atoi(strmsg[0].c_str());
    cmd.set_to_id(toid);
    cmd.set_content(strmsg[1]);
    cmd.set_content_len(strmsg[1].size());

    chat::ChatMsgHead head;
    head.set_type(chat::msg_type::chat_private);
    head.set_req(chat::msg_req::request);

    chatroom::ChatMsg msg;
    msg.setHead(head);
    msg.setMsg(&cmd);

    if (sendMsgtoServer(msg) < 0)
    {
        std::cout << "发送消息失败" << std::endl;
    }
}

void ChatClient::listUser()
{

}

void ChatClient::listRecord()
{

}

int ChatClient::sendMsgtoLocal(chatroom::ChatMsg& msg)
{
    std::string senstr = msg.getStrAll();
    unsigned int sendsum = 0;
    while (sendsum < senstr.size())
    {
        int cursend = write(_fd[0], senstr.c_str() + sendsum, senstr.size() - sendsum);
        if (cursend == -1)
        {
            return -1;
        }
        else if (cursend >= 0)
        {
            sendsum += cursend;
        }
    }
    return 0;
}

int ChatClient::getMsgfromLocal(chatroom::ChatMsg &msg)
{
    std::string readStr;
    char buff[4096];
    int nread = 0;
    if ((nread = read(_fd[1], buff, 4096)) < 0)
    {
        return -1;
    }

    readStr += std::string(buff,nread);
	
    if ( !msg.getFromStr(readStr))
    {
        return -1;
    }
    return 0;
}

int ChatClient::sendMsgtoServer(chatroom::ChatMsg& msg)
{
    std::string senstr = msg.getStrAll();
    unsigned int sendsum = 0;
    while (sendsum < senstr.size())
    {
        int cursend = write(_sockFd, senstr.c_str() + sendsum, senstr.size() - sendsum);
        if (cursend == -1)
        {
            return -1;
        }
        else if (cursend >= 0)
        {
            sendsum += cursend;
        }
    }
    
    return 0;
}

int ChatClient::getMsgfromServer(chatroom::ChatMsg &msg)
{
    std::string readStr;
    char buff[4096];
    int nread = 0;
    if ((nread = read(_sockFd, buff, 4096)) < 0)
    {
        return -1;
    }

    readStr += std::string(buff,nread);
	
    if ( !msg.getFromStr(readStr))
    {
        return -1;
    }
    return 0;
}

int ChatClient::cmdEnterChatRoom()
{
    chat::Cmd cmd;
    cmd.set_uid(0);
    cmd.set_cmdtpye(chat::cmd_type::enter_room);

    chat::ChatMsgHead head;
    head.set_type(chat::msg_type::cmd);
    head.set_req(chat::msg_req::request);

    chatroom::ChatMsg msg;
    msg.setHead(head);
    msg.setMsg(&cmd);

    if (sendMsgtoServer(msg) < 0)
    {
        return -1;
    }

    chatroom::ChatMsg msgr;
    if (getMsgfromLocal(msgr) < 0)
    {
        return -1;
    }
    chat::Cmd cmdr;
    cmdr.ParseFromString(msgr.getBodyStr());
    if (cmdr.status() == chat::ret_status::failed)
    {
        return -1;
    }
    return 0;
}

int ChatClient::cmdLeaveChatRoom()
{
        chat::Cmd cmd;
    cmd.set_uid(0);
    cmd.set_cmdtpye(chat::cmd_type::leave_room);

    chat::ChatMsgHead head;
    head.set_type(chat::msg_type::cmd);
    head.set_req(chat::msg_req::request);

    chatroom::ChatMsg msg;
    msg.setHead(head);
    msg.setMsg(&cmd);

    if (sendMsgtoServer(msg) < 0)
    {
        return -1;
    }

    chatroom::ChatMsg msgr;
    if (getMsgfromLocal(msgr) < 0)
    {
        return -1;
    }
    chat::Cmd cmdr;
    cmdr.ParseFromString(msgr.getBodyStr());
    if (cmdr.status() == chat::ret_status::failed)
    {
        return -1;
    }
    return 0;
}

int ChatClient::cmdWelcome()
{
    chat::Cmd cmd;
    cmd.set_uid(0);
    cmd.set_cmdtpye(chat::cmd_type::welcome);

    chat::ChatMsgHead head;
    head.set_type(chat::msg_type::cmd);
    head.set_req(chat::msg_req::request);

    chatroom::ChatMsg msg;
    msg.setHead(head);
    msg.setMsg(&cmd);

    if (sendMsgtoServer(msg) < 0)
    {
        return -1;
    }

    chatroom::ChatMsg msgr;
    if (getMsgfromLocal(msgr) < 0)
    {
        return -1;
    }
    chat::Cmd cmdr;
    cmdr.ParseFromString(msgr.getBodyStr());
    if (cmdr.status() == chat::ret_status::failed)
    {
        return -1;
    }
    return 0;
}