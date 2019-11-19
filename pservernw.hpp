#ifndef POKER_HPP
#define POKER_HPP
#include <string>

extern int server_fd;
extern int new_socket; 
extern char buffer[1024];

void sendstring(int socket, std::string text);
std::string recvstring(int socket);


#endif