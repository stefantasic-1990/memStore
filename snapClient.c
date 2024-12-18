#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "craftLine.h"

char* readSocket(int socket_fd) {
    int bufferSize = 50;
    char* inputBuffer = malloc(bufferSize);

    char c;
    int i = 0;
    while (1) {
        read(socket_fd, &c, 1);
        if (c == '\0') {
            inputBuffer[i] = c;
            return inputBuffer;
        } else {
            inputBuffer[i++] = c;
        }

        if (i >= bufferSize) {
            bufferSize += 50;
            inputBuffer = realloc(inputBuffer, bufferSize);
        }
    }
}

int main(int argc, char** argv) {
    int client_fd;
    struct sockaddr_in serverAddress;
    char* server_ip = "127.0.0.1";

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    if (inet_pton(AF_INET, server_ip, &serverAddress.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server at %s:%d\n", server_ip, 8080);

    while (1) {
        char* messageBuffer;
        messageBuffer = craftLine("snapClient => ");
        
        if (strcmp(messageBuffer, "exit") == 0) {
            printf("Exiting client program.\n");
            break;
        }

        if (send(client_fd, messageBuffer, strlen(messageBuffer), 0) < 0) {
            perror("Send failed");
            break;
        }

        messageBuffer = readSocket(client_fd);
        
        printf("Server response: %s\n", messageBuffer);
    }

    close(client_fd);
    return 0;
}