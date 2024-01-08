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
    int sockfd, newsockfd;
    socklen_t clientLength;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr, clientAddr;

    // Server mode
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        error("Error on binding");
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    printf("Server listening on port %d...\n", PORT);

    clientLength = sizeof(clientAddr);

    // Accept connection from client
    newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLength);
    if (newsockfd < 0) {
        error("Error on accept");
    }

    printf("Connection established with client\n");

    while (1) {
        // Server reads from client
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(newsockfd, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            error("Error reading from socket");
        }

        if (bytesRead == 0) {
            printf("Client disconnected. Exiting...\n");
            break;
        }

        printf("Client: %s", buffer);

        // Server writes to client
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        int bytesWritten = write(newsockfd, buffer, strlen(buffer));
        if (bytesWritten < 0) {
            error("Error writing to socket");
        }
    }

    // Close sockets
    close(sockfd);
    close(newsockfd);

    return 0;
}
