#pragma once
#include "../lion/Server.h"


namespace chatroom
{
    class ChatServer : public lion::Server
    {
    public:
        ChatServer(int port, int subLoopNum, lion::EventFactory *eventFactory);
        ~ChatServer();
        
    private:
        
    };

}
