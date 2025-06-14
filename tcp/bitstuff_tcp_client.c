#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

// Function to perform bit stuffing (adds 0 after "011111")
void bit_stuffing(char *input, char *stuffed) {
    int i, j = 0;

    for (i = 0; input[i] != '\0'; i++) {
        stuffed[j] = input[i];

        // If last 6 bits match "011111", insert a '0' right after
        if (i >= 5 && strncmp(&input[i - 5], "011111", 6) == 0) {
            stuffed[++j] = '0'; // Stuff a '0' after the pattern
        }
        j++;
    }

    stuffed[j] = '\0'; // Null-terminate stuffed data
}

int main() {
    int sockfd;
    struct sockaddr_in address;
    char input_data[MAX_SIZE], stuffed_data[MAX_SIZE], received_data[MAX_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server details (localhost and port 9734)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    address.sin_port = htons(9734); // Port number

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    // Take binary string input from user
    printf("Enter a binary string: ");
    scanf("%s", input_data);

    // Perform bit stuffing on input
    bit_stuffing(input_data, stuffed_data);
    printf("After Bit Stuffing: %s\n", stuffed_data);

    // Send stuffed data to server
    int bytes_sent = write(sockfd, stuffed_data, strlen(stuffed_data) + 1);
    if (bytes_sent == -1) {
        perror("Write error");
        close(sockfd);
        exit(1);
    }

    // Clear buffer and receive de-stuffed data from server
    memset(received_data, 0, MAX_SIZE);
    int bytes_received = read(sockfd, received_data, MAX_SIZE - 1);
    if (bytes_received <= 0) {
        perror("Read error");
    } else {
        received_data[bytes_received] = '\0'; // Null-terminate response
        printf("De-Stuffed Data received from Server: %s\n", received_data);
    }

    // Close the socket connection
    close(sockfd);
    return 0;
}
