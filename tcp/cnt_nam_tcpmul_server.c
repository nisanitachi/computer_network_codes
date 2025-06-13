#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set address properties
    address.sin_family = AF_INET;            // IPv4
    address.sin_addr.s_addr = INADDR_ANY;    // Accept connections from any IP address
    address.sin_port = htons(PORT);          // Port number in network byte order

    // Bind the socket to the address and port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen for incoming connections
    listen(server_fd, MAX_CLIENTS);

    printf("Server listening on port %d...\n", PORT);

    // Infinite loop to accept and handle clients
    while (1) {
        // Accept a client connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Fork a child process to handle this client
        if (fork() == 0) {
            // Inside child process
            close(server_fd); // Child doesn't need the main listening socket

            // Clear buffer and receive name from client
            memset(buffer, 0, BUFFER_SIZE);
            recv(new_socket, buffer, BUFFER_SIZE, 0);
            printf("Received name: %s\n", buffer);

            // Calculate string length
            int len = strlen(buffer);
            char msg[BUFFER_SIZE];
            sprintf(msg, "Length: %d", len); // Create response message

            // Send length back to client
            send(new_socket, msg, strlen(msg), 0);

            // Close client connection and terminate child process
            close(new_socket);
            exit(0);
        }

        // Parent process closes the client socket (already handled by child)
        close(new_socket);
    }

    return 0;
}
