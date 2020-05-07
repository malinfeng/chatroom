#include "chatclient.h"

int main()
{
        ChatClient *cli = new ChatClient("127.0.0.1", 8500);
        cli->start();
}