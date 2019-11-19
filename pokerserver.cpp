
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include "pservernw.hpp"
#include "pcards.hpp"
#include "gameround.hpp"

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
    server_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
}

int yes = 1;
if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
}

if (bind(server_fd, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
{
    std::cerr << "Bind error: \n" << status;
    exit(1);
}

if (listen(server_fd, 8) != 0)
{
    std::cerr << "Listen error: \n" << status;
    exit(1);
}

socklen_t addr_size = sizeof theirinfo;

new_socket = accept(server_fd, (struct sockaddr*)&theirinfo, &addr_size);

freeaddrinfo(servinfo);


    for (int i = 1; i <= 52; ++i)
    {    
        deck.push_back(i);
    }

    playerlist.emplace_back(Player());
    playerlist.emplace_back(Player());

    while (playerlist.size() > 1)
    {
        Gameround round = Gameround();
        round.roundrun();
    }
    return 0; 
} 
