#pragma once

#include "ChatEvent.h"
#include "../lion/EventFactory.h"

namespace chatroom
{
    class ChatEventFactory :
		public lion::EventFactory
	{
	public:
		ChatEventFactory()
		{}
		~ChatEventFactory()
		{}
		std::shared_ptr<lion::Event> createTcpEvent(lion::EventLoop *loop, int fd, lion::NetAddr *cliaddr)
		{
			std::shared_ptr<lion::Event> ret = std::make_shared<ChatEvent>(loop, fd);
			ret->enableReading();
			return ret;
		}

	};
}
