#include "ChatEvent.h"

#include "../lion/Buffer.h"
#include "CommuManager.h"

using namespace lion;
using namespace chatroom;

ChatEvent::ChatEvent(lion::EventLoop *loop, int fd)		
        :
			Event(loop, fd),
			_pconnection(new TcpConnection(fd)),
            _uid(0)
{
    
}

ChatEvent::~ChatEvent()
{

}

void ChatEvent::readEvent()
{
    int readsum = _pconnection->readFd();
    if (readsum > 0)
    {
        handleOp();
    }
    else
    {
        //连接关闭,用户下线
        if (_uid > 0)
        {
            CommuManeger::GetInstnce()->delLivingUser(_uid);
        }

        EventLoop *thisloop = getLoop();
		thisloop->addTaskInLoop(std::bind(&EventLoop::deleteEvent, thisloop, shared_from_this()));
    }
    
}

void ChatEvent::writeEvent()
{
    while (_pconnection->outStrSize() > 0)
    {
        if(_pconnection->sendMsg() <= 0)
        {
            break;
        }
    }
    
    if(_pconnection->outStrSize() > 0)
    {
        enableWriting();
    }
    else
    {
        disableWriting();
    }
    
}

void ChatEvent::errorEvent()
{
    
}

void ChatEvent::closeEvent()
{
    
}

void ChatEvent::handleOp()
{
    std::shared_ptr<Buffer> buf = _pconnection->getInBuffer();

    std::string tempstr = buf->getAllString();
    _curmsg.getFromStr(tempstr);

    switch (_curmsg.getHead().type())
    {
        case chat::msg_type::welcome :
        handleWelcome();
        break;

        case chat::msg_type::login :
        handleLogin();
        break;

        case chat::msg_type::list_friend :
        handleListFriend();
        break;

        case chat::msg_type::list_record :
        handleListRecord();
        break;

        case chat::msg_type::add_friend :
        handleAddFriend();
        break;

        case chat::msg_type::delete_friend :
        handleDelFriend();
        break;

        case chat::msg_type::chat_broadcast :
        handleChatBroadcast();
        break;

        case chat::msg_type::chat_private :
        handleChatPrivate();
        break;

        default:

        break;
    }
}

void ChatEvent::handleWelcome()
{
    
}

void ChatEvent::handleLogin()
{
    chat::LoginReq login;
    login.ParseFromString(_curmsg.getBodyStr());

    //login处理逻辑
    //此处基类被shared_from_this管理，子类需要类型转换，否则会报错
    _uid = login.uid();
    CommuManeger::GetInstnce()->addLivingUser(_uid, std::dynamic_pointer_cast<ChatEvent>(shared_from_this()));

    //login回复逻辑
    chat::LoginRsp loginrmsg;
    loginrmsg.set_status(1);

    chat::ChatMsgHead loginrhead;
    loginrhead.set_type(chat::msg_type::login);
    loginrhead.set_req(chat::msg_req::response);
    
    ChatMsg loginr;
    loginr.setHead(loginrhead);
    loginr.setMsg(&loginrmsg);

    std::string sedstr = loginr.getStrAll();
    sendMsg(sedstr);
}

void ChatEvent::handleListFriend()
{

}

void ChatEvent::handleListRecord()
{

}

void ChatEvent::handleAddFriend()
{

}

void ChatEvent::handleDelFriend()
{

}

void ChatEvent::handleChatBroadcast()
{

}

void ChatEvent::handleChatPrivate()
{

}

void ChatEvent::sendMsg(std::string &msg)
{
    getLoop()->addTaskInLoop(std::bind(&ChatEvent::HelpSendMsg, this, msg));
}


void ChatEvent::HelpSendMsg(std::string &msg)
{
    int sendsize = msg.size();
    _pconnection->appendMsg(msg);
    if (_pconnection->sendMsg() < sendsize)
    {
        enableWriting();
    }
}
