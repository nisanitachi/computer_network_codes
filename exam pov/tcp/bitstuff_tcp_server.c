#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

// Function to perform bit de-stuffing (removes 0 after "011111")
void bit_destuffing(char *stuffed, char *destuffed) {
    int i, j = 0;
    for (i = 0; stuffed[i] != '\0'; i++) {
        destuffed[j] = stuffed[i];
        // Check if the last 6 bits match "011111" and a '0' follows it
        if (i >= 5 && strncmp(&stuffed[i - 5], "011111", 6) == 0 && stuffed[i + 1] == '0') {
            i++; // Skip the stuffed '0'
        }
        j++;
    }
    destuffed[j] = '\0'; // Null-terminate the destuffed string
}

int main() {
    int server_sockfd, client_sockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len;
    char stuffed_data[MAX_SIZE], destuffed_data[MAX_SIZE];

    // Create TCP socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Assign IP and port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    server_address.sin_port = htons(9734); // Port number

    // Bind socket to the specified IP and port
    if (bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Start listening for connections
    listen(server_sockfd, 5);
    printf("Server started. Waiting for client...\n");

    while (1) {
        client_len = sizeof(client_address);
        
        // Accept incoming client connection
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        if (client_sockfd == -1) {
            perror("Accept failed");
            continue;
        }
        printf("Client connected.\n");

        // Clear input/output buffers
        memset(stuffed_data, 0, MAX_SIZE);
        memset(destuffed_data, 0, MAX_SIZE);

        // Read stuffed bit stream from client
        int bytes_received = read(client_sockfd, stuffed_data, MAX_SIZE - 1);
        if (bytes_received <= 0) {
            perror("Read error");
            close(client_sockfd);
            continue;
        }

        // Null-terminate received string
        stuffed_data[bytes_received] = '\0';
        printf("Received Stuffed Data: %s\n", stuffed_data);

        // Perform bit de-stuffing operation
        bit_destuffing(stuffed_data, destuffed_data);
        printf("After De-Stuffing: %s\n", destuffed_data);

        // Send de-stuffed bit stream back to the client
        int bytes_sent = write(client_sockfd, destuffed_data, strlen(destuffed_data) + 1);
        if (bytes_sent == -1) {
            perror("Write error");
        }

        // Close the connection with the client
        close(client_sockfd);
        printf("Client disconnected. Waiting for new connection...\n");
    }

    // Close the server socket (not reached in this infinite loop)
    close(server_sockfd);
    return 0;
}
