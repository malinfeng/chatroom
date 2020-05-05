# Install script for directory: /home/malinfeng/chatroom/lion

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/malinfeng/chatroom/build/lib/liblion.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/malinfeng/chatroom/build/lib" TYPE STATIC_LIBRARY FILES "/home/malinfeng/chatroom/build/lion/liblion.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/malinfeng/chatroom/build/include/Buffer.h;/home/malinfeng/chatroom/build/include/config.h;/home/malinfeng/chatroom/build/include/Epoll.h;/home/malinfeng/chatroom/build/include/Event.h;/home/malinfeng/chatroom/build/include/EventFactory.h;/home/malinfeng/chatroom/build/include/EventLoop.h;/home/malinfeng/chatroom/build/include/ListenEvent.h;/home/malinfeng/chatroom/build/include/MainEventLoop.h;/home/malinfeng/chatroom/build/include/NetAddr.h;/home/malinfeng/chatroom/build/include/Server.h;/home/malinfeng/chatroom/build/include/SocketOps.h;/home/malinfeng/chatroom/build/include/SubEventLoopPool.h;/home/malinfeng/chatroom/build/include/SubEventLoopThread.h;/home/malinfeng/chatroom/build/include/TcpConnection.h;/home/malinfeng/chatroom/build/include/TcpConnectionEvent.h;/home/malinfeng/chatroom/build/include/ThreadPool.h;/home/malinfeng/chatroom/build/include/TimerEvent.h;/home/malinfeng/chatroom/build/include/TimerQueue.h;/home/malinfeng/chatroom/build/include/TimesTamp.h;/home/malinfeng/chatroom/build/include/WakeupEvent.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/malinfeng/chatroom/build/include" TYPE FILE FILES
    "/home/malinfeng/chatroom/lion/Buffer.h"
    "/home/malinfeng/chatroom/lion/config.h"
    "/home/malinfeng/chatroom/lion/Epoll.h"
    "/home/malinfeng/chatroom/lion/Event.h"
    "/home/malinfeng/chatroom/lion/EventFactory.h"
    "/home/malinfeng/chatroom/lion/EventLoop.h"
    "/home/malinfeng/chatroom/lion/ListenEvent.h"
    "/home/malinfeng/chatroom/lion/MainEventLoop.h"
    "/home/malinfeng/chatroom/lion/NetAddr.h"
    "/home/malinfeng/chatroom/lion/Server.h"
    "/home/malinfeng/chatroom/lion/SocketOps.h"
    "/home/malinfeng/chatroom/lion/SubEventLoopPool.h"
    "/home/malinfeng/chatroom/lion/SubEventLoopThread.h"
    "/home/malinfeng/chatroom/lion/TcpConnection.h"
    "/home/malinfeng/chatroom/lion/TcpConnectionEvent.h"
    "/home/malinfeng/chatroom/lion/ThreadPool.h"
    "/home/malinfeng/chatroom/lion/TimerEvent.h"
    "/home/malinfeng/chatroom/lion/TimerQueue.h"
    "/home/malinfeng/chatroom/lion/TimesTamp.h"
    "/home/malinfeng/chatroom/lion/WakeupEvent.h"
    )
endif()

