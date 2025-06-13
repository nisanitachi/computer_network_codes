#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "parity_socket"
#define BUFFER_SIZE 100

// Function to count number of 1s in the bit stream
int countOnes(char *bitstream) {
    int count = 0;
    for (int i = 0; bitstream[i] != '\0'; i++) {
        if (bitstream[i] == '1') {
            count++;
        }
    }
    return count;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    // Create Unix domain socket
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Remove any existing socket file
    unlink(SOCKET_PATH);

    // Set socket address structure
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections
    listen(server_sock, 5);
    printf("Server is listening...\n");

    // Accept a client connection
    client_sock = accept(server_sock, NULL, NULL);
    if (client_sock < 0) {
        perror("Accept failed");
        exit(1);
    }

    // Receive bit stream from client
    read(client_sock, buffer, sizeof(buffer));
    printf("Received from client: %s\n", buffer);

    // Add odd parity
    int ones = countOnes(buffer);
    int len = strlen(buffer);
    if (ones % 2 == 0) {
        buffer[len] = '1'; // Add 1 if number of 1s is even to make it odd
    } else {
        buffer[len] = '0'; // Already odd, add 0
    }
    buffer[len + 1] = '\0';

    // Send back modified stream to client
    write(client_sock, buffer, strlen(buffer) + 1);
    printf("Sent to client (with odd parity): %s\n", buffer);

    // Clean up
    close(client_sock);
    close(server_sock);
    unlink(SOCKET_PATH);

    return 0;
}
