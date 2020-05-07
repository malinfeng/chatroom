#pragma once

#include <string>

#include "../../lion/Event.h"
#include "../../lion/TcpConnection.h"
#include "../../lion/EventLoop.h"

#include "../../build/src/chat.pb.h"
#include "../ChatMsg.h"

namespace chatroom
{
	class ChatEvent : public lion::Event
	{
	public:
        ChatEvent(lion::EventLoop *loop, int fd);
		~ChatEvent();

		//多线程接口，使用addtask方式
		void sendMsg(std::string &msg);

	private:
		void readEvent();
		void writeEvent();
		void errorEvent();
		void closeEvent();

		
		void handleOp();

		void handleCmd();
		void handleLogin();
		void handleListRecord();
		void handleChatBroadcast();
		void handleChatPrivate();


		//发送消息的回调
		void HelpSendMsg(std::string &msg);
		
	private:
		std::shared_ptr<lion::TcpConnection> _pconnection;

		unsigned int _uid;

		ChatMsg _curmsg;
    };
}