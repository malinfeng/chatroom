
include(FindProtobuf)

# include_directories(${PROTOBUF_INCLUDE_DIR})

# include_directories(${PROJECT_SOURCE_DIR}/lion)

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER chat.proto)


aux_source_directory(./server serverSrc)
add_executable(server 
    ${serverSrc}
    ChatMsg.cpp 
    ${PROTO_SRC})

aux_source_directory(./client clientSrc)
add_executable(client 
    ${clientSrc}
    ChatMsg.cpp 
    ${PROTO_SRC})

target_link_libraries(server lion protobuf)

target_link_libraries(client lion protobuf)