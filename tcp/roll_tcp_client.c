// TCP Client: Sends roll numbers to server and receives sorted list
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 6

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int roll_numbers[SIZE];

    // Take input from user
    printf("Enter 6 roll numbers:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Roll number %d: ", i + 1);
        scanf("%d", &roll_numbers[i]);
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);  // Change if needed

    // Connect to server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Send roll numbers to server
    send(sock, roll_numbers, sizeof(roll_numbers), 0);

    // Receive sorted roll numbers
    read(sock, roll_numbers, sizeof(roll_numbers));

    // Print sorted roll numbers
    printf("Client: Sorted roll numbers: ");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", roll_numbers[i]);
    printf("\n");

    // Close socket
    close(sock);

    return 0;
}
