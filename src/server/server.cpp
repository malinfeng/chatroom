#include "ChatEventFactory.h"
#include "ChatServer.h"

using namespace chatroom;

int main ()
{
    lion::EventFactory *factory = new ChatEventFactory();
	chatroom::ChatServer *myserver = new ChatServer(8500, 4, factory);
	myserver->start();
    return 0;
}