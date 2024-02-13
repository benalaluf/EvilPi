#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>
#include "conn/ConnData.h"

int createTCPIPv4Socket();

struct sockaddr_in *createIPv4Address(const char *ip, int port);

struct sockaddr_in *createIPv4Address(int sockFD);

ConnData acceptConn(int sockFD);