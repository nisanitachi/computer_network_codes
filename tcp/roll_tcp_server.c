// TCP Server: Sorts roll numbers received from client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define SIZE 6

void sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int roll_numbers[SIZE];

    // Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Setup server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accept from any IP
    address.sin_port = htons(PORT);

    // Bind the socket to the address and port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Start listening
    listen(server_fd, 3);
    printf("Server is listening on port %d...\n", PORT);

    // Accept a client connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // Read roll numbers from client
    read(new_socket, roll_numbers, sizeof(roll_numbers));

    // Sort the array
    sort(roll_numbers, SIZE);

    // Print sorted roll numbers on server side
    printf("Server: Sorted roll numbers: ");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", roll_numbers[i]);
    printf("\n");

    // Send sorted array back to client
    send(new_socket, roll_numbers, sizeof(roll_numbers), 0);

    // Close sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
