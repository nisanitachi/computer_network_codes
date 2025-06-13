// converter_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "converter_socket"

int main() {
    int sockfd, number;
    struct sockaddr_un server_addr;
    char result[100];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    // Get decimal number from user
    printf("Enter a decimal number: ");
    scanf("%d", &number);

    // Send number to server
    write(sockfd, &number, sizeof(number));

    // Receive conversion result
    read(sockfd, result, sizeof(result));

    // Display result
    printf("Conversions received from server:\n%s", result);

    close(sockfd);
    return 0;
}
