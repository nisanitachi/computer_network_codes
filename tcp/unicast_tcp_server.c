// Server Code: TCP Chat using Threads with Loopback (127.0.0.1)

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

// Global client file descriptor
int cfd;

int main() {
    struct sockaddr_in ser, cl;
    int sfd, len, n;
    pthread_t snd, rcv;

    len = sizeof(cl);

    // Step 1: Create server socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Configure server socket address
    ser.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(ser.sin_addr)); // Loopback address for localhost
    ser.sin_port = htons(8760);              // Port number

    // Step 3: Bind socket to IP and port
    n = bind(sfd, (struct sockaddr *)&ser, sizeof(ser));

    // Step 4: Start listening for connections
    n = listen(sfd, 1);
    printf("Server listening on 127.0.0.1:8760...\n");

    // Step 5: Accept a client connection
    cfd = accept(sfd, (struct sockaddr *)&cl, &len);
    printf("Client connected.\n");

    // Step 6: Create threads for sending and receiving messages
    pthread_create(&snd, NULL, send_thread, NULL);
    pthread_create(&rcv, NULL, recv_thread, NULL);

    // Step 7: Wait for both threads to finish
    pthread_join(snd, NULL);
    pthread_join(rcv, NULL);

    // Step 8: Close connection
    close(cfd);
    return 0;
}

// Thread function for sending messages to client
void *send_thread(void *a) {
    char str[200];
    while (1) {
        fgets(str, 200, stdin);  // Read user input
        write(cfd, (void *)str, sizeof(str));  // Send to client

        if (strncmp(str, "bye", 3) == 0) {
            printf("Disconnected from client.\n");
            exit(0);  // Exit on "bye"
        }
    }
    pthread_exit(NULL);
}

// Thread function for receiving messages from client
void *recv_thread(void *a) {
    int n;
    char str[200];
    while (1) {
        n = read(cfd, (void *)str, sizeof(str));  // Read from client

        if (n > 0) {
            printf("Client: %s", str);
            fflush(stdout);  // Flush output buffer
        }

        if (strncmp(str, "bye", 3) == 0) {
            printf("Client has disconnected.\n");
            exit(0);  // Exit on "bye"
        }
    }
    pthread_exit(NULL);
}
