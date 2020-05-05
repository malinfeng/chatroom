#include <assert.h>

#include "Event.h"
#include "SocketOps.h"

using namespace lion;

const int Event::kReadEvent = EPOLLIN | EPOLLPRI;
const int Event::kWriteEvent = EPOLLOUT;


Event::Event(EventLoop* loop, int fd)
	:
	_loop(loop),
	_fd(fd),
	_events(0),
	_revents(0),
	_added(false)
{
	assert(fd < MAXFD);
	if (USE_ET_MOD)
	{
		_events |= EPOLLET;
		assert(setSocketNonBlocking(_fd) == 0);
	}
}


Event::~Event()
{
	close(_fd);
}


void Event::handleEvents()
{
	//对端关闭时，一般同时收到EPOLLHUP和EPOLLIN，因此当作读事件处理（读到0）
	if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
	{
		closeEvent();//也可以认为是挂起事件，一般不会触发
	}
	if (_revents & EPOLLERR)
	{
		errorEvent();
	}
	if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		readEvent();
	}
	if (_revents & EPOLLOUT)
	{
		writeEvent();
	}
	//警告，未知事件
}
