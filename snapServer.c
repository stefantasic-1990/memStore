#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <arpa/inet.h>
#include "snapDB.h"

char* parseCommand(char* command) {
    char* commandName;
    char* commandArg;
    char* result;

    char *spacePos = strchr(command, ' ');
    if (spacePos == NULL) {
        return "Command must be composed of a method and argument";
    } else {
        int separator = spacePos - command;
        commandName = malloc(separator + 1);
        strncpy(commandName, command, separator);
        commandName[separator] = '\0';
        commandArg = strdup(spacePos + 1);
    }

    if (strcmp(commandName, "get") == 0) {
        result = get(commandArg);
        if (result != NULL) {
            return result;
        } else {
            return "Key does not exist.";
        }
    } else if (strcmp(commandName, "remove") == 0) {
        if (remove(commandArg) == 0) {
            return "Success";
        } else {
            return "Key does not exist";
        }
    } else if (strcmp(commandName, "put") == 0) {
        char* commandArg1 = strtok(commandArg, ":");
        char* commandArg2 = strtok(NULL, ":");
        if (put(commandArg1, commandArg2) == 0) {
            return "Success";
        } else {
            return "Unable to store key";
        }
    } else {
        return NULL;
    }
}

char* readMessage(int socket_fd) {
    int bufferSize = 150;
    char* inputBuffer = malloc(bufferSize);
    int i = 0;
    char c;
    while (1) {
        int bytesRead = read(socket_fd, &c, 1);
        if (bytesRead == 0) {
            printf("Client disconnected gracefully\n");
            inputBuffer = NULL;
            break;
        } else if (bytesRead == -1) {
            perror("Read error, disconnecting client");
            inputBuffer = NULL;
            break;
        }

        if (c == '\0') {
            inputBuffer[i] = c;
            break;
        } else {
            inputBuffer[i++] = c;
        }

        if (i >= bufferSize) {
            bufferSize += 50;
            inputBuffer = realloc(inputBuffer, bufferSize);
        }
    }

    return inputBuffer;
}

int initializeServer() {
    printf("Initializing server...\n");

    int socket_fd;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setting soocket option 'REUSEADDR' failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080); 

    if (bind(socket_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Binding the socket failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, 3) < 0) {
        perror("Unable to listen on socket. Closing");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Server sucessfully initialized.\n");
    return socket_fd;
}

int main() {
    int server_fd = initializeServer();
    if (initializeDatabase() != 0) {
        perror("Database failed to initiailize");
        exit(EXIT_FAILURE);
    }

    int client_fd;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    do {
        printf("Listening for new connection...\n");
        if ((client_fd = accept(server_fd, (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddressLen)) < 0) {
            perror("Accepting an incoming connection has failed");
            continue;
        }
        printf("Connection established with client\n");

        char* messageBuffer;
        do {
            messageBuffer = readMessage(client_fd);
            if (messageBuffer == NULL) {
                close(client_fd);
                break;
            } else {
                messageBuffer = parseCommand(messageBuffer);
                if (messageBuffer != NULL) {
                    write(client_fd, messageBuffer, strlen(messageBuffer)+1);
                } else {
                    write(client_fd, "error, invalid command", sizeof("error, invalid command"));
                }
            }
        } while(1);
    } while (1);

}