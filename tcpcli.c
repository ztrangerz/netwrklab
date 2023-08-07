#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server_address;
    char message[MAX_BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Read input from the user
    printf("Enter a message: ");
    fgets(message, MAX_BUFFER_SIZE, stdin);

    // Send the message to the server
    send(sock, message, strlen(message), 0);

    // Receive the response from the server
    char server_response[MAX_BUFFER_SIZE] = {0};
    read(sock, server_response, MAX_BUFFER_SIZE);
    printf("Server response: %s", server_response);

    close(sock);

    return 0;
}
