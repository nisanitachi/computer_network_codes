#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "server_socket" // Path for the UNIX domain socket

// Function to sort an array using Bubble Sort
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap elements if they're in the wrong order
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int server_sockfd, client_sockfd;
    struct sockaddr_un server_address, client_address;
    socklen_t client_len;

    // Remove the socket file if it already exists
    unlink(SOCKET_PATH);

    // Step 1: Create a UNIX domain stream socket
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Step 2: Configure server socket address structure
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);

    // Step 3: Bind the socket to the specified address
    if (bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // Step 4: Listen for incoming client connections
    if (listen(server_sockfd, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server waiting for client connection...\n");

    // Step 5: Accept a connection from a client
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
    if (client_sockfd == -1) {
        perror("Client connection failed");
        exit(1);
    }

    printf("Client connected!\n");

    // Step 6: Get input from the user (server-side) for demonstration
    int n;
    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Step 7: Sort the array
    sort(arr, n);

    // Step 8: Display the sorted array on the server
    printf("Sorted array in server: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Step 9: Send the sorted array to the client
    write(client_sockfd, &n, sizeof(n));       // Send size of array
    write(client_sockfd, arr, sizeof(arr));    // Send sorted array data

    // Step 10: Close connections and clean up
    close(client_sockfd);
    close(server_sockfd);
    unlink(SOCKET_PATH);  // Remove the socket file

    return 0;
}
