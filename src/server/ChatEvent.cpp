#include "ChatEvent.h"

#include "../../lion/Buffer.h"
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
    std::string tempstr = _pconnection->getInBuffer()->getAllString();
    _curmsg.getFromStr(tempstr);

    switch (_curmsg.getHead().type())
    {
        case chat::msg_type::cmd :
        handleCmd();
        break;

        case chat::msg_type::login :
        handleLogin();
        break;

        case chat::msg_type::list_record :
        handleListRecord();
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

void ChatEvent::handleCmd()
{
    //命令选项
    chat::Cmd cmd;
    cmd.ParseFromString(_curmsg.getBodyStr());

    switch (cmd.cmdtpye())
    {
        case chat::cmd_type::welcome :
            cmd.set_status(chat::ret_status::sucess);
        break;

        case chat::cmd_type::enter_room :
            if (_uid)
            {
                cmd.set_status(chat::ret_status::sucess);
                CommuManeger::GetInstnce()->enterChatRoom(_uid);
            }
            else
            {
                cmd.set_status(chat::ret_status::failed);
            }
        break;


        case chat::cmd_type::leave_room :
            if (_uid)
            {
                cmd.set_status(chat::ret_status::sucess);
                CommuManeger::GetInstnce()->leaveChatRoom(_uid);
            }
            else
            {
                cmd.set_status(chat::ret_status::failed);
            }
            
            
        break;

        default : break;
    }

    chat::ChatMsgHead cmdRHead;
    cmdRHead.set_type(chat::msg_type::cmd);
    cmdRHead.set_req(chat::msg_req::response);

    ChatMsg cmdR;
    cmdR.setHead(cmdRHead);
    cmdR.setMsg(&cmd);

    std::string sedstr = cmdR.getStrAll();
    sendMsg(sedstr);
}

void ChatEvent::handleLogin()
{
    //登陆，根据id登陆账号
    chat::Login login;
    login.ParseFromString(_curmsg.getBodyStr());

    //login处理逻辑
    //此处基类被shared_from_this管理，子类需要类型转换，否则会报错
    _uid = login.uid();
    CommuManeger::GetInstnce()->addLivingUser(_uid, std::dynamic_pointer_cast<ChatEvent>(shared_from_this()));



    //login回复逻辑
    login.set_status(chat::ret_status::sucess);

    chat::ChatMsgHead loginhead;
    loginhead.set_type(chat::msg_type::login);
    loginhead.set_req(chat::msg_req::response);
    
    ChatMsg loginr;
    loginr.setHead(loginhead);
    loginr.setMsg(&login);

    std::string sedstr = loginr.getStrAll();
    sendMsg(sedstr);
}


void ChatEvent::handleListRecord()
{

}


void ChatEvent::handleChatBroadcast()
{
    chat::ChatPublic chatPub;
    chatPub.ParseFromString(_curmsg.getBodyStr());
    if (chatPub.content().size() != chatPub.content_len())
    {
        //消息出错
        return;
    }


    //公开聊天处理逻辑


    //公开聊天回复逻辑
    chat::ChatMsgHead pubhead;
    pubhead.set_type(chat::msg_type::chat_broadcast);
    pubhead.set_req(chat::msg_req::response);
    
    ChatMsg chatpPubr;
    chatpPubr.setHead(pubhead);
    chatpPubr.setMsg(&chatPub);

    std::string sedstr = chatpPubr.getStrAll();
    CommuManeger::GetInstnce()->sendMsgToPub(sedstr);
}

void ChatEvent::handleChatPrivate()
{
    chat::ChatPrivate chatPri;
    chatPri.ParseFromString(_curmsg.getBodyStr());
    if (chatPri.content().size() != chatPri.content_len())
    {
        //消息出错
        return;
    }
    //私聊处理逻辑


    //私聊回复逻辑
    chat::ChatMsgHead prihead;
    prihead.set_type(chat::msg_type::chat_private);
    prihead.set_req(chat::msg_req::response);
    
    ChatMsg chatpPrir;
    chatpPrir.setHead(prihead);
    chatpPrir.setMsg(&chatPri);

    std::string sedstr = chatpPrir.getStrAll();
    CommuManeger::GetInstnce()->sendMsgToOne(chatPri.to_id(), sedstr);
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
