#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE], recv_buffer[BUFFER_SIZE];

    // Create a TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Define server address
    server_address.sin_family = AF_INET;          // IPv4
    server_address.sin_port = htons(PORT);        // Port number
    server_address.sin_addr.s_addr = INADDR_ANY;  // Server IP

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Take user input (name)
    printf("Enter your name: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline character

    // Send the name to the server
    send(sock, buffer, strlen(buffer), 0);

    // Receive and print the response from the server
    memset(recv_buffer, 0, BUFFER_SIZE);
    recv(sock, recv_buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", recv_buffer);

    // Close the socket
    close(sock);

    return 0;
}
