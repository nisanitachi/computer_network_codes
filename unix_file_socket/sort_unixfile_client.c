#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "server_socket" // Must match the server's socket path

int main() {
    int sockfd;
    struct sockaddr_un address;
    int result, n;

    // Step 1: Create a UNIX domain stream socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Step 2: Configure the server's address
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    // Step 3: Connect to the server
    result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    if (result == -1) {
        perror("Connection failed");
        exit(1);
    }

    // Step 4: Receive the number of integers from the server
    read(sockfd, &n, sizeof(n));

    int arr[n];  // Declare array of size 'n' to store received integers

    // Step 5: Receive the sorted array from the server
    read(sockfd, arr, sizeof(arr));

    // Step 6: Display the sorted array
    printf("Sorted array received in client: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Step 7: Close the socket
    close(sockfd);

    return 0;
}
