#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr;

    // Client mode
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    printf("Enter server IP address: ");
    char serverIP[20];
    scanf("%s", serverIP);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        error("Error connecting to server");
    }

    printf("Connected to server\n");

    while (1) {
        // Client writes to server
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        int bytesWritten = write(sockfd, buffer, strlen(buffer));
        if (bytesWritten < 0) {
            error("Error writing to socket");
        }

        // Client reads from server
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(sockfd, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            error("Error reading from socket");
        }

        if (bytesRead == 0) {
            printf("Server disconnected. Exiting...\n");
            break;
        }

        printf("Server: %s", buffer);
    }

    // Close socket
    close(sockfd);

    return 0;
}
