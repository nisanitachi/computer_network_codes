// TCP Chatroom Client
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Thread function declarations
void *recv_thread(void *a);  // Receives messages from server
void *send_thread(void *a);  // Sends messages to server

int cfd;                     // Client socket file descriptor
pthread_t sample_t;          // Thread ID for receiving messages

int main() {
    struct sockaddr_in cl, ser;
    pthread_t snd, rcv;

    // Create socket
    cfd = socket(AF_INET, SOCK_STREAM, 0);

    // Clear memory of sockaddr_in structs
    memset(&cl, 0, sizeof(cl));
    memset(&ser, 0, sizeof(ser));

    // Set client address details (optional)
    cl.sin_family = AF_INET;
    inet_aton("10.10.107.168", &(cl.sin_addr));  // Client IP
    cl.sin_port = htons(8760);                   // Client port

    // Set server address details
    ser.sin_family = AF_INET;
    inet_aton("10.10.107.168", &(ser.sin_addr));  // Server IP
    ser.sin_port = htons(8760);                   // Server port

    // Connect to the server
    if (connect(cfd, (struct sockaddr *)&ser, sizeof(ser)) < 0) {
        printf("Unable to establish connection!\n");
        exit(0);
    }

    // Create sender and receiver threads
    pthread_create(&snd, NULL, send_thread, NULL);
    pthread_create(&rcv, NULL, recv_thread, NULL);

    // Wait for sender thread to finish
    pthread_join(snd, NULL);
    return 0;
}

// This thread handles sending messages to the server
void *send_thread(void *a) {
    char str[200];

    while (1) {
        fgets(str, 200, stdin);  // Get user input
        write(cfd, (void *)str, sizeof(str));  // Send message to server

        // If user types !DISCONNECT, close the socket and exit
        if (strcmp(str, "!DISCONNECT\n") == 0) {
            pthread_cancel(sample_t);  // Stop receive thread
            close(cfd);
            exit(0);
        }
    }
}

// This thread handles receiving messages from the server
void *recv_thread(void *a) {
    char str[200];
    sample_t = pthread_self();  // Store this thread’s ID

    while (1) {
        int n = read(cfd, (void *)str, sizeof(str));
        if (n > 0) {
            printf("Server: %s", str);
            fflush(stdout);
        }
    }
}
