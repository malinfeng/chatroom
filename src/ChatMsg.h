#pragma once

#include <string>
#include "../build/src/chat.pb.h"
#include <google/protobuf/message.h>

namespace chatroom
{
    class ChatMsg
    {
    public:
        ChatMsg();
        ~ChatMsg();
        bool getFromStr(std::string& str);

        void setHead(chat::ChatMsgHead head);
        void setMsg(google::protobuf::Message* body);
        std::string getStrAll();

        chat::ChatMsgHead& getHead() {return _head; }
        std::string getBodyStr();

    private:
        chat::ChatMsgHead _head;
        //使用str方式存body
        std::string _bodyStr;
        bool _hasbody;
    };
}