
// Client side C/C++ program to demonstrate Socket programming 
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <signal.h>
#include <string.h> 
#include <string>
#include <cstring>
#include <vector>
#define PORT 8080 

int sock = 0; 
char buffer[1024] = {0}; 


void sendstring(std::string text)
{
    std::string test = text;
    int length = test.length();
    test = std::to_string(length) + '=' + test;
    char * uus = new char [test.length()+1];
    strcpy(uus, test.c_str());
    if (int tmp = send(sock , uus , strlen(uus) , 0) == -1)
        std::cerr << "Send error.\n";
}

std::string recvstring()
{
    static int still_in_buffer = 0;
    if (still_in_buffer == 0)
    {
        memset(buffer, '\0', 1024);
        while (buffer[0] == '\0')
        {
            if (int valread = read(sock, buffer, 1024) == -1)
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

void clientcardchanger()
{
    std::cout << "How many cards do you want to change?\n";
    int num = -1;
    while (num < 0 || num > 5)
    {
        if (!(std::cin >> num))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (num < 0 || num > 5)
            std::cout << "Number between 0 and 5.";
    }
    std::string tmp = std::to_string(num);
    sendstring(tmp);
    if (num != 5)
    {
        for (int i = 0; i < num; ++i)
        {
            std::cout << "Which card do you want to toss?\n";
            int pos = -1;
            while (pos < 1 || pos > 5 - i)
            {
                    if (!(std::cin >> pos))
                    {
                        std::cout << "Error, try again." << "\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
            }
            std::string tmp = "";
            tmp = std::to_string(pos);
            sendstring(tmp);
            std::cout << recvstring();
        }
    }
    std::cout << "Your new hand is: \n";
}
void craise()
{
    int minraise = std::stoi(recvstring());
    int raise = -1;
    while (raise < minraise)
    {
        if (!(std::cin >> raise))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (raise < minraise)
            std::cout << "That raise is too small.\n";
    }
    sendstring(std::to_string(raise));
}

void clientturn()
{
    std::cout << "Choose to fold(f), call/check(c) or raise(r): \n";
    std::string choice = "";
    while (choice != "f" && choice != "c" && choice != "r")
    {
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    sendstring(choice);
}

int main()
{

int status;

struct addrinfo ai;
struct addrinfo *res, *p;
char ipstr[INET6_ADDRSTRLEN];
struct addrinfo* servinfo;
memset(&ai, 0, sizeof ai);
ai.ai_family = AF_UNSPEC;
ai.ai_socktype = SOCK_STREAM;
ai.ai_flags = AI_PASSIVE;


struct sockaddr aa;

struct sockaddr_storage theirinfo;


if ((status = getaddrinfo(NULL, "3490", &ai, &servinfo)) != 0) 
{
    std::cerr << "Getaddrinfo error: \n" << gai_strerror(status);
    exit(1);
}
else
{
    sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
}


if (connect(sock, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
{
    std::cerr << "Connect error: \n" << gai_strerror(status);
    exit(1);
}

freeaddrinfo(servinfo);


// Here is the game for client:
    while (true)
    {
        std::string command = "";
        command = recvstring();
        if (command == "CARDCHANGER")
        {
            clientcardchanger();
        }
        else if (command == "TURN")
        {
            clientturn();
        }
        else if (command == "RAISE") 
        {
            craise();
        }
        else
        {
            std::cout << command << "\n";
        }    
    }

    return 0; 
} 
