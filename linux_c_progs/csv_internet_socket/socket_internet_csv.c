#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define CSV_FILE_PATH "/home/devarshi.trivedi/workspace/linux_c_progs/csv_internet_socket/example.csv"

// Function to send an HTTP response
void sendHttpResponse(int sockfd, const char *response) {
    send(sockfd, response, strlen(response), 0);
}

// Function to handle the client request
void handleRequest(int sockfd, const char *request) {
    // Parse the request to extract the parameter (e.g., /1)
    char param[10];
    sscanf(request, "GET /%s HTTP", param);

    // Open the CSV file
    FILE *csvFile = fopen(CSV_FILE_PATH, "r");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        exit(EXIT_FAILURE);
    }

    // Search for the record with the specified parameter
    char line[MAX_BUFFER_SIZE];
    char *foundRecord = NULL;
    while (fgets(line, sizeof(line), csvFile) != NULL) {
        if (strstr(line, param) != NULL) {
            foundRecord = strdup(line);
            break;
        }
    }

    // Close the CSV file
    fclose(csvFile);

    // Prepare the HTTP response
    char response[MAX_BUFFER_SIZE];
    if (foundRecord != NULL) {
        sprintf(response, "HTTP/1.1 200 OK\r\n\r\n%s", foundRecord);
        free(foundRecord);
    } else {
        sprintf(response, "HTTP/1.1 404 Not Found\r\n\r\nRecord not found");
    }

    // Send the HTTP response
    sendHttpResponse(sockfd, response);
}

int main() {
    const int port = 8080;
    const char *ip_address = "10.0.17.115";

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d...\n", ip_address, port);

    while (1) {
        // Accept a connection
        int client_sock = accept(sockfd, NULL, NULL);
        if (client_sock < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Read the HTTP request from the client
        char request[MAX_BUFFER_SIZE];
        int bytes_received = recv(client_sock, request, sizeof(request) - 1, 0);
        if (bytes_received < 0) {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }
        request[bytes_received] = '\0';

        // Handle the client request
        handleRequest(client_sock, request);

        // Close the client socket
        close(client_sock);
    }

    // Close the server socket
    close(sockfd);

    return 0;
}
