#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  // For socket functions and structures
#include <unistd.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"  // Localhost
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE], recv_buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    servaddr.sin_family = AF_INET;                 // IPv4
    servaddr.sin_port = htons(SERVER_PORT);        // Server port
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP); // Server IP

    // Get bit stream input from user
    printf("Enter bit stream (only 0s and 1s): ");
    scanf("%s", buffer);

    // Send bit stream to server
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive modified bit stream with parity from server
    recvfrom(sockfd, recv_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);
    recv_buffer[strlen(recv_buffer)] = '\0';  // Ensure null-termination

    // Display result to user
    printf("Received bit stream with even parity: %s\n", recv_buffer);

    close(sockfd);  // Close the socket
    return 0;
}
