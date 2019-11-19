#include <string.h> 
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "pservernw.hpp"
#include <iostream>

int server_fd;
int new_socket; 
char buffer[1024] = {0}; 

void sendstring(int socket, std::string text)
{
    std::string test = text;
    int length = test.length();
    test = std::to_string(length) + '=' + test;
    char * uus = new char [test.length()+1];
    strcpy(uus, test.c_str());
    if (int tmp = send(socket , uus , strlen(uus) , 0) == -1)
        std::cerr << "Send error.\n";
}

std::string recvstring(int socket)
{
    static int still_in_buffer = 0;
    if (still_in_buffer == 0)
    {
        memset(buffer, '\0', 1024);
        while (buffer[0] == '\0')
        {
            if (int valread = read(socket, buffer, 1024) == -1)
                std::cerr << "Receive error.\n";
        }
    }
    std::string message(buffer);
    std::string len = "";
    int i = still_in_buffer;
    while (true)
    {
        if (message[i] == '=')
            break;
        len += message[i];
        ++i;
    }
    int length = std::stoi(len);
    if (message.length() > i + 1 + length)
        still_in_buffer = i + 1 + length;
    else
        still_in_buffer = 0;
    std::string themessage = "";
    for (int j = i + 1; j < i + 1 + length; ++j)
        themessage += message[j];
    return themessage;
}