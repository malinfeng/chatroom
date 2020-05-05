include(FindProtobuf)

# include_directories(${PROTOBUF_INCLUDE_DIR})


# include_directories(${PROJECT_SOURCE_DIR}/lion)


protobuf_generate_cpp(PROTO_SRC PROTO_HEADER chat.proto)



add_executable(server 
    server.cpp 
    ChatEvent.cpp 
    ChatServer.cpp 
    ChatMsg.cpp 
    CommuManager.cpp
    ${PROTO_SRC})

add_executable(client 
    client.cpp 
    ChatMsg.cpp 
    ${PROTO_SRC})

target_link_libraries(server lion protobuf)

target_link_libraries(client protobuf pthread)