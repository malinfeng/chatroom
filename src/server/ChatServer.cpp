#include "ChatServer.h"

using namespace chatroom;

ChatServer::ChatServer(int port, int subLoopNum,  lion::EventFactory *eventFactory)
    : 
    lion::Server(port, subLoopNum, eventFactory)   
{

}

ChatServer::~ChatServer()
{   

}