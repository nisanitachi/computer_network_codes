#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  // For socket functions and structures
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024

// Function to calculate and add even parity bit to a bit stream
void add_even_parity(char *bitstream, char *result) {
    int ones = 0;

    // Count number of 1s in the bit stream
    for (int i = 0; bitstream[i] != '\0'; i++) {
        if (bitstream[i] == '1') ones++;
    }

    // Copy the original bitstream
    strcpy(result, bitstream);

    // Append parity bit (0 if count of 1s is even, 1 if odd)
    if (ones % 2 == 0)
        strcat(result, "0");
    else
        strcat(result, "1");
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE], result[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    servaddr.sin_family = AF_INET;            // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;    // Accept from any IP
    servaddr.sin_port = htons(PORT);          // Server port

    // Bind socket to the IP and port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is running and waiting for client...\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer before receiving

        // Receive bit stream from client
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        printf("Received bit stream: %s\n", buffer);

        // Add even parity bit
        add_even_parity(buffer, result);
        printf("Sending back with even parity: %s\n", result);

        // Send back the modified bit stream to the client
        sendto(sockfd, result, strlen(result), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);  // Close the socket
    return 0;
}
