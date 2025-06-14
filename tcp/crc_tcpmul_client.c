// client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);  // Convert IP

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("[-] Connection failed");
        return 1;
    }

    // Take user input
    char dataword[MAX], divisor[MAX];
    printf("Enter dataword: ");
    scanf("%s", dataword);
    printf("Enter divisor: ");
    scanf("%s", divisor);

    // Prepare message to send
    char message[MAX];
    snprintf(message, sizeof(message), "%s %s", dataword, divisor);

    // Send data to server
    send(sock, message, strlen(message), 0);

    // Receive CRC codeword from server
    char codeword[MAX] = {0};
    recv(sock, codeword, MAX, 0);

    // Display received codeword
    printf("[✓] Received Codeword from Server: %s\n", codeword);

    // Close the socket
    close(sock);

    return 0;
}
