// even_odd_server.c

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "even_odd_socket"

int main() {
    int server_sockfd, client_sockfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    int number;
    char result[10];

    // Remove old socket file if exists
    unlink(SOCKET_PATH);

    // Create socket
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Bind socket
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections
    listen(server_sockfd, 5);
    printf("Server waiting for client...\n");

    // Accept client connection
    client_len = sizeof(client_addr);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sockfd == -1) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    // Read number from client
    read(client_sockfd, &number, sizeof(number));
    printf("Received number: %d\n", number);

    // Check even or odd
    if (number % 2 == 0)
        strcpy(result, "Even");
    else
        strcpy(result, "Odd");

    // Send result back to client
    write(client_sockfd, result, sizeof(result));

    // Close sockets and clean up
    close(client_sockfd);
    close(server_sockfd);
    unlink(SOCKET_PATH);

    return 0;
}
