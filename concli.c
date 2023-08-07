#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define MAX_BUFFER_SIZE 1024

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];
    time_t currentTime;
    struct tm *timeInfo;
    char timeString[MAX_BUFFER_SIZE];

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
        // Receive data from client
        memset(buffer, 0, sizeof(buffer));
        recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrLen);

        printf("Received request from client %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        if (strcmp(buffer, "TIME_REQUEST") == 0) {
            // Get current time
            currentTime = time(NULL);
            timeInfo = localtime(&currentTime);

            // Convert time to string
            strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);

            // Send time to the client
            sendto(serverSocket, timeString, strlen(timeString), 0, (struct sockaddr *)&clientAddr, addrLen);
        }
    }

    close(serverSocket);
    return 0;
}
