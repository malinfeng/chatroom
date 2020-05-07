#include "CommuManager.h"

using namespace chatroom;

CommuManeger* CommuManeger::_sglton = NULL;

CommuManeger::CommuManeger()
{

}

CommuManeger::~CommuManeger()
{

}

void CommuManeger::sendMsgToPub(std::string& msg)
{
    for (auto info : _livingClientMap)
    {
        if (info.second._status == clistatus::outRoom)
        {
            continue;
        }
        else
        {
            info.second._sp_ev->sendMsg(msg);
        }
    }
}

void CommuManeger::sendMsgToOne(unsigned int uid, std::string& msg)
{
    if (_livingClientMap.count(uid))
    {
        _livingClientMap[uid]._sp_ev->sendMsg(msg);
    }
}

bool CommuManeger::addLivingUser(unsigned int uid, std::shared_ptr<ChatEvent> sp_ev)
{
    if (_livingClientMap.count(uid))
    {
        return false;
    }
    ClientInfo tempCinfo(sp_ev);

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _livingClientMap[uid] = tempCinfo;
    }
    
    return true;
}

bool CommuManeger::delLivingUser(unsigned int uid)
{
    if (!_livingClientMap.count(uid))
    {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _livingClientMap.erase(uid);
    }
    
    return true;
}

void CommuManeger::enterChatRoom(unsigned int uid)
{
    if (_livingClientMap.count(uid))
    {
        _livingClientMap[uid]._status = clistatus::inroom;
    }
}

void CommuManeger::leaveChatRoom(unsigned int uid)
{
    if (_livingClientMap.count(uid))
    {
        _livingClientMap[uid]._status = clistatus::outRoom;
    }
}