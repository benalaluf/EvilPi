#pragma once

#include <string>
#include <netinet/in.h>

void startRSHSession(int sockFD);

pid_t startRSHSessionPipe(int sockfd,struct sockaddr_in *src, struct sockaddr_in *dst, int *toShellPipe, int *fromShellPipe);


void recvRSHSSession(int sockFD);

void sendRSHCommand(std::string command, int *toShellPipe);