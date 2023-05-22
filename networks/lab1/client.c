#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);
    char buffer[MAX_BUFFER_SIZE];

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket err");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("inet_pton err");
        exit(1);
    }

    while (1) {
        printf("enter message to send ('q' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        if (strcmp(buffer, "q\n") == 0)
            break;

        ssize_t sentLen = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, addrLen);
        if (sentLen == -1) {
            perror("sendto err");
            exit(1);
        }

        ssize_t recvLen = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (recvLen == -1) {
            perror("recvfrom err");
            exit(1);
        }

        printf("response from server: %s\n", buffer);
    }

    // Close socket
    close(clientSocket);

    return 0;
}
