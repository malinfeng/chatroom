#include "ChatMsg.h"

using namespace chatroom;

ChatMsg::ChatMsg()
{
    _hasbody = false;
}

ChatMsg::~ChatMsg()
{

}

bool ChatMsg::getFromStr(std::string& str)
{
    if (str.size() <= 4)
    {
        return false;
    }
    std::string len_str = str.substr(0, 4);
    int len = *(int*)len_str.c_str();

    if(!_head.ParseFromString(str.substr(4, len)))
    {
        return false;
    }
    if (_head.msg_len() > 0)
    {
        _hasbody = true;
        _bodyStr = str.substr(4 + len);
    }
    else
    {
        _hasbody = false;
        _bodyStr.clear();
    }
    return true;
}

void ChatMsg::setHead(chat::ChatMsgHead head)
{
    _head = head;
}

void ChatMsg::setMsg(google::protobuf::Message* body)
{
    _head.set_msg_len(body->ByteSizeLong());
    body->SerializeToString(&_bodyStr);
}

std::string ChatMsg::getStrAll()
{
    int headsize = _head.ByteSizeLong();
    char* hsize = (char*)&headsize;

    //变包长,前四个字节记录包长
    std::string bufhead;
    _head.SerializeToString(&bufhead);

    std::string ret = std::string(hsize, 4) + bufhead + _bodyStr;
    return ret;
}

std::string ChatMsg::getBodyStr()
{
    return _bodyStr;
}