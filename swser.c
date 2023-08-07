#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define MAX_BUFFER_SIZE 1024

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];
    int expectedSeqNumber = 0;

    // Create UDP socket
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        // Receive data from the client
        memset(buffer, 0, sizeof(buffer));
        recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrLen);

        int receivedSeqNumber;
        sscanf(buffer, "%d", &receivedSeqNumber);

        if (receivedSeqNumber == expectedSeqNumber) {
            printf("Received message: %s\n", buffer + 2); // Skip the sequence number and colon
            printf("Sending ACK for sequence number: %d\n", expectedSeqNumber);

            // Send ACK back to the client
            sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, addrLen);

            // Update expected sequence number
            expectedSeqNumber = (expectedSeqNumber + 1) % 2;
        }
    }

    close(serverSocket);
    return 0;
}
