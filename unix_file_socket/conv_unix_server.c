// converter_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "converter_socket"

int main() {
    int server_sockfd, client_sockfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    int number;
    char binary[33], octal[20], hex[20], response[100];

    // Remove old socket file
    unlink(SOCKET_PATH);

    // Create socket
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Bind socket
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connection
    listen(server_sockfd, 5);
    printf("Server ready. Waiting for client...\n");

    client_len = sizeof(client_addr);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sockfd == -1) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    // Read decimal number from client
    read(client_sockfd, &number, sizeof(number));
    printf("Received number: %d\n", number);

    // Convert to binary
    int temp = number, i = 0;
    char bin[33];
    do {
        bin[i++] = (temp % 2) + '0';
        temp /= 2;
    } while (temp > 0);
    bin[i] = '\0';
    // Reverse binary string
    for (int j = 0; j < i / 2; j++) {
        char t = bin[j];
        bin[j] = bin[i - j - 1];
        bin[i - j - 1] = t;
    }
    strcpy(binary, bin);

    // Convert to octal and hexadecimal
    sprintf(octal, "%o", number);
    sprintf(hex, "%X", number);

    // Prepare result string
    snprintf(response, sizeof(response),
             "Binary: %s\nOctal: %s\nHexadecimal: %s\n",
             binary, octal, hex);

    // Send back to client
    write(client_sockfd, response, strlen(response) + 1);

    // Clean up
    close(client_sockfd);
    close(server_sockfd);
    unlink(SOCKET_PATH);

    return 0;
}
