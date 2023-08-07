#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept incoming connections and handle clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected.\n");

        // Save the new socket to the client_sockets array
        client_sockets[i] = new_socket;
    }

    // Handle chat messages from clients
    while (1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (recv(client_sockets[i], buffer, BUFFER_SIZE, 0) > 0) {
                // Broadcast the received message to all other clients
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (j != i && client_sockets[j] != 0) {
                        send(client_sockets[j], buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }

    return 0;
}
