#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define MAX_BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);
    char buffer[MAX_BUFFER_SIZE];
    int sequenceNumber = 0;

    // Create UDP socket
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter a message to send: ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);

    // Send data to the server using Stop-and-Wait protocol
    while (1) {
        // Send the message with the sequence number
        sprintf(buffer, "%d:%s", sequenceNumber, buffer);
        sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, addrLen);

        // Receive ACK from the server
        memset(buffer, 0, sizeof(buffer));
        recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addrLen);

        // Check if the ACK received matches the expected sequence number
        int receivedSeqNumber;
        sscanf(buffer, "%d", &receivedSeqNumber);
        if (receivedSeqNumber == sequenceNumber) {
            printf("Message sent successfully\n");
            break;
        }
    }

    close(clientSocket);
    return 0;
}
