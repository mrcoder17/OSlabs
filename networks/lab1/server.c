#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8888

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind err");
        exit(1);
    }

    printf("UDP serv started on port %d\n", SERVER_PORT);

    while (1) {
        ssize_t recvLen = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (recvLen == -1) {
            perror("recvfrom err");
            exit(1);
        }

        printf("data from client: %s", buffer);

        ssize_t sentLen = sendto(serverSocket, buffer, recvLen, 0, (struct sockaddr*)&clientAddr, addrLen);
        if (sentLen == -1) {
            perror("sendto err");
            exit(1);
        }

        printf("back data to client: %s", buffer);
    }

    // Close socket
    close(serverSocket);

    return 0;
}
