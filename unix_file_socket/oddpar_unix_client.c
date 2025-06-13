#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "parity_socket"
#define BUFFER_SIZE 100

int main() {
    int sockfd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Take input from user
    printf("Enter bit stream (e.g., 10101): ");
    scanf("%s", buffer);

    // Send to server
    write(sockfd, buffer, strlen(buffer) + 1);

    // Receive response from server
    read(sockfd, buffer, sizeof(buffer));
    printf("Received from server (with odd parity): %s\n", buffer);

    // Clean up
    close(sockfd);
    return 0;
}
