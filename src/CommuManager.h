#pragma once
#include <map>

#include <memory>
#include <mutex> 

#include "ChatEvent.h"

namespace chatroom
{
    enum clistatus
    {
        nochat = 0,
        pub,
        pri
    };

    struct ClientInfo
    {
        ClientInfo(std::shared_ptr<ChatEvent> sp_ev):
            _sp_ev(sp_ev)
        {
            _status = clistatus::nochat;
        }
        ClientInfo(){}
        std::shared_ptr<ChatEvent> _sp_ev;
        int _status;
    };

    //通信管理类,单例类,保证线程安全
    class CommuManeger
    {
    public:

        static CommuManeger* GetInstnce()
        {
            if (_sglton == NULL)
            {
                _sglton = new CommuManeger;
            }
            return _sglton;
        }

        void sendMsgToPub(std::string& msg);
        void sendMsgToOne(unsigned int uid, std::string& msg);

        bool addLivingUser(unsigned int uid, std::shared_ptr<ChatEvent> sp_ev);
        bool delLivingUser(unsigned int uid);

    private:
        CommuManeger();
        ~CommuManeger();

    private:
        //在线用户，uid与用户信息的映射
        std::map<unsigned int, ClientInfo> _livingClientMap;

        static CommuManeger* _sglton;

        std::mutex _mutex;
    };
}
