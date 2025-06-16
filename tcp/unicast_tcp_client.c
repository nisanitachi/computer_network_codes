// Client Code: TCP Chat using Threads with Loopback (127.0.0.1)

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Function declarations
void *recv_thread(void *a);
void *send_thread(void *a);

// Global socket descriptor
int cfd;

int main() {
    struct sockaddr_in cl;
    pthread_t snd, rcv;

    // Step 1: Create client socket
    cfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Set up server address (loopback)
    cl.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(cl.sin_addr));  // Server IP (localhost)
    cl.sin_port = htons(8760);               // Server port

    // Step 3: Connect to the server
    connect(cfd, (struct sockaddr *)&cl, sizeof(cl));
    printf("Connected to server at 127.0.0.1:8760\n");

    // Step 4: Create threads for sending and receiving
    pthread_create(&snd, NULL, send_thread, NULL);
    pthread_create(&rcv, NULL, recv_thread, NULL);

    // Step 5: Wait for threads to finish
    pthread_join(snd, NULL);
    pthread_join(rcv, NULL);

    // Step 6: Close socket
    close(cfd);
    return 0;
}

// Thread function to send messages to the server
void *send_thread(void *a) {
    char str[200];
    while (1) {
        fgets(str, 200, stdin);  // Input from user
        write(cfd, (void *)str, sizeof(str));  // Send to server

        if (strncmp(str, "bye", 3) == 0) {
            printf("Disconnected from server.\n");
            exit(0);  // Exit on "bye"
        }
    }
    pthread_exit(NULL);
}

// Thread function to receive messages from the server
void *recv_thread(void *a) {
    int n;
    char str[200];
    while (1) {
        n = read(cfd, (void *)str, sizeof(str));  // Read from server

        if (n > 0) {
            printf("Server: %s", str);
            fflush(stdout);
        }

        if (strncmp(str, "bye", 3) == 0) {
            printf("Server has disconnected.\n");
            exit(0);  // Exit on "bye"
        }
    }
    pthread_exit(NULL);
}
