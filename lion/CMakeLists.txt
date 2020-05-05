
aux_source_directory(. source_files)

add_library(lion ${source_files})

find_package(Threads REQUIRED)
target_link_libraries(lion Threads::Threads)

install(TARGETS lion DESTINATION ${PROJECT_BINARY_DIR}/lib/)


SET(HEADERS
    Buffer.h
    config.h
    Epoll.h
    Event.h
    EventFactory.h
    EventLoop.h
    ListenEvent.h
    MainEventLoop.h
    NetAddr.h
    Server.h
    SocketOps.h
    SubEventLoopPool.h
    SubEventLoopThread.h
    TcpConnection.h
    TcpConnectionEvent.h
    ThreadPool.h
    TimerEvent.h
    TimerQueue.h
    TimesTamp.h
    WakeupEvent.h
    )

install(FILES ${HEADERS} DESTINATION ${PROJECT_BINARY_DIR}/include/)