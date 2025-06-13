// TCP Multi-client Sender (Chatroom Server)
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXCLIENT 3  // Maximum number of clients allowed

// Global declarations
struct sockaddr_in ser, cli;             // Server and client socket structures
int sfd, cfd[MAXCLIENT], temp = 0;       // Server socket, client sockets, client count
pthread_t sample_t[MAXCLIENT];           // To track client receive threads

// Thread function declarations
void *recv_thread(void *arg);  // Handles receiving messages from a specific client
void *send_thread(void *arg);  // Handles broadcasting messages from server to all clients

int main() {
    int len, num;
    pthread_t snd, rcv[MAXCLIENT];

    // Create server socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set server socket details
    ser.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(ser.sin_addr));  // Localhost IP
    ser.sin_port = htons(8760);               // Port number

    // Bind the server socket
    num = bind(sfd, (struct sockaddr*)&ser, sizeof(ser));
    printf("Chatroom Ready!\n");

    // Start listening for client connections
    num = listen(sfd, MAXCLIENT);
    len = sizeof(cli);

    // Accept up to MAXCLIENT clients
    while (temp < MAXCLIENT) {
        // Accept client connection
        cfd[temp] = accept(sfd, (struct sockaddr*)&cli, &len);
        printf("Client[%d] Joined!\n", temp + 1);

        // Pass client index to recv_thread
        int *ptr = (int *)malloc(sizeof(int));
        *ptr = temp;

        // Start the send_thread only once (for broadcasting from server to all clients)
        if (temp == 0)
            pthread_create(&snd, NULL, send_thread, NULL);

        // Start receive thread for this specific client
        pthread_create(&rcv[temp], NULL, recv_thread, (void *)ptr);

        temp++;  // Move to next client slot
    }

    // Wait for send thread to finish
    pthread_join(snd, NULL);
    return 0;
}

// Thread to broadcast server messages to all connected clients
void *send_thread(void *arg) {
    char str[200];
    int pos;

    while (1) {
        pos = 0;
        fgets(str, 200, stdin);  // Get input from server terminal

        // If server types "!DISCONNECT", terminate all receive threads and close server
        if (strcmp(str, "!DISCONNECT\n") == 0) {
            while (pos <= temp - 1) {
                if (pthread_cancel(sample_t[pos]) == 0)
                    pos++;
            }
            close(sfd);
            exit(0);
        } else {
            // Broadcast message to all clients
            while (pos <= temp - 1) {
                write(cfd[pos], (void *)str, sizeof(str));
                pos++;
            }
        }
    }
}

// Thread to receive messages from a specific client
void *recv_thread(void *arg) {
    int n;
    char str[200];
    int pos = *((int *)arg);  // Client index
    sample_t[pos] = pthread_self();  // Store thread ID

    while (1) {
        n = read(cfd[pos], (void *)str, sizeof(str));
        if (n > 0) {
            // If client sends "!DISCONNECT"
            if (strcmp(str, "!DISCONNECT\n") == 0) {
                printf("Client[%d] Left!\n", pos + 1);
            } else {
                // Display client message
                printf("Client[%d]: %s", pos + 1, str);
            }
            fflush(stdout);
        }
    }
}
