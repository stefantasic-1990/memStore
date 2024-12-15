#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    struct sockaddr_in serverAddress;
    char* server_ip = "127.0.0.1";
    char message[1024];

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
        printf("Enter message to send to server: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "exit") == 0) {
            printf("Exiting client program.\n");
            break;
        }

        if (send(client_fd, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }

        int bytesRead = read(client_fd, message, sizeof(message));
        if (bytesRead > 0) {
            message[bytesRead] = '\0';
            printf("Server response: %s\n", message);
        }
    }

    close(client_fd);
    return 0;
}