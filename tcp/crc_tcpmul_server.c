// server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 1024

// XOR operation for CRC
void xor(char *remainder, const char *divisor, int len) {
    for (int i = 0; i < len; i++) {
        remainder[i] = (remainder[i] == divisor[i]) ? '0' : '1';
    }
}

// CRC calculation logic
void computeCRC(const char *dataword, const char *divisor, char *codeword) {
    int data_len = strlen(dataword);
    int divisor_len = strlen(divisor);
    char temp[MAX], remainder[MAX];

    // Append (divisor_len - 1) zeros to dataword
    strcpy(temp, dataword);
    for (int i = 0; i < divisor_len - 1; i++) {
        temp[data_len + i] = '0';
    }
    temp[data_len + divisor_len - 1] = '\0';

    // Copy the first 'divisor_len' bits to remainder
    strncpy(remainder, temp, divisor_len);
    remainder[divisor_len] = '\0';

    // Perform division
    for (int i = 0; i < data_len; i++) {
        if (remainder[0] == '1') {
            xor(remainder, divisor, divisor_len);  // XOR if MSB is 1
        } else {
            for (int j = 0; j < divisor_len; j++) {
                remainder[j] = (j == 0) ? '0' : remainder[j];
            }
        }

        // Left shift and bring in next bit
        memmove(remainder, remainder + 1, divisor_len - 1);
        remainder[divisor_len - 1] = temp[i + divisor_len];
        remainder[divisor_len] = '\0';
    }

    // Combine dataword and remainder to form codeword
    strcpy(codeword, dataword);
    strncat(codeword, remainder, divisor_len - 1);
}

// Thread function for each client
void *handle_client(void *arg) {
    int new_socket = *(int *)arg;
    free(arg);  // Free dynamically allocated memory

    char buffer[MAX] = {0};
    recv(new_socket, buffer, MAX, 0);  // Receive input from client

    printf("\n[+] Client connected.\n");
    printf("[>] Received from client: %s\n", buffer);

    // Parse dataword and divisor from received buffer
    char dataword[MAX], divisor[MAX];
    sscanf(buffer, "%s %s", dataword, divisor);
    printf("[>] Dataword: %s | Divisor: %s\n", dataword, divisor);

    // Compute CRC codeword
    char codeword[MAX];
    computeCRC(dataword, divisor, codeword);
    printf("[✓] Computed Codeword: %s\n", codeword);

    // Send codeword back to client
    send(new_socket, codeword, strlen(codeword), 0);

    close(new_socket);  // Close client socket
    pthread_exit(NULL); // Exit thread
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Server socket setup
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to IP/port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Start listening for incoming connections
    listen(server_fd, 5);
    printf("[*] Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

        // Allocate memory for client socket
        int *pclient = malloc(sizeof(int));
        *pclient = new_socket;

        // Create a new thread to handle the client
        pthread_t t;
        pthread_create(&t, NULL, handle_client, pclient);
        pthread_detach(t);  // Auto clean up after thread finishes
    }

    return 0;
}
