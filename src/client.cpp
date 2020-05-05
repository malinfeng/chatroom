#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>
#include <unistd.h>
#include "../build/src/chat.pb.h"
#include "ChatMsg.h"

int testlogin(int hsock)
{

        chat::LoginReq login;
        login.set_uid(20);
        login.set_uname("malinfeng");

 
        chat::ChatMsgHead loginhead;
        loginhead.set_type(chat::msg_type::login);
        loginhead.set_req(chat::msg_req::request);

        chatroom::ChatMsg loginmsg;
        loginmsg.setHead(loginhead);
        loginmsg.setMsg(&login);

        std::string sendstr;
        sendstr = loginmsg.getStrAll();

        int sendcount = 0;
        if( (sendcount = send(hsock, (void *) sendstr.c_str(), sendstr.size(), 0))== -1 ) 
        {
                printf("Error sending data %d\n", errno);
                return - 1;
        }
        printf("Sent bytes %d\n", sendcount);

        char buf[4096];
        bzero(buf, 4096);
        int recvcount = 0;
        if( (recvcount = recv(hsock, buf, 4096, 0)) == -1 ) 
        {
                printf("Error recv data %d\n", errno);
                return - 1;
        }
        printf("Recv bytes %d\n", sendcount);

        std::string recvstr(buf, recvcount);

        chatroom::ChatMsg loginRspMsg;
        loginRspMsg.getFromStr(recvstr);

        chat::ChatMsgHead loginrsqhead = loginRspMsg.getHead();

        chat::LoginRsp loginrsq;
        loginrsq.ParseFromString(loginRspMsg.getBodyStr());

        return 0;
}


int main()
{
        int hsock = socket(AF_INET, SOCK_STREAM, 0);
        if(hsock == -1){
                printf("Error initializing socket %d\n",errno);
                return -1;
        }

        int op = 1;
        if( setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(int)) == -1 )
        {
                printf("Error setting options %d\n",errno);
                return -1;
        }

        int server_port= 8500;
        const char* server_name = "127.0.0.1";
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET ;
        server_addr.sin_port = htons(server_port);
        server_addr.sin_addr.s_addr = inet_addr(server_name);

        if( connect( hsock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 )
        {
                printf("Error connecting socket %d\n", errno);
                return -1;
        }

        testlogin(hsock);

        return 0;
}
