#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in serverAddress, clientAddress;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setting soocket option 'REUSEADDR' failed");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080); 

    if (bind(server_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Binding the socket failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Unable to listen on socket. Closing");
        exit(EXIT_FAILURE);
    }
    
    socklen_t clientAddressLen = sizeof(clientAddress);
    do {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddressLen)) < 0) {
            perror("Accepting an incoming connection has failed");
            continue;
        }
        printf("Connection established with client\n");

        char buffer[1024];
        int bytesRead;

        while ((bytesRead = read(client_fd, buffer, sizeof(buffer))) > 0) {
            buffer[bytesRead] = '\0';
            printf("Received command from client: %s\n", buffer);
            write(client_fd, "Message received", 17);
        }

        if (bytesRead == 0) {
            printf("Client disconnected gracefully\n");
        } else if (bytesRead == -1) {
            perror("Read error. Disconnecting the client");
        }

        close(client_fd);

    } while (1);

}