// Hexadecimal Receiver Program
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/msg.h>  

#define MAX_TEXT 512  

// Define the message structure
struct my_msg_st {  
    long int my_msg_type;  
    char some_text[MAX_TEXT];  
};  

int main() {  
    int running = 1;  
    int msgid;  
    struct my_msg_st some_data;  
    long int msg_to_receive = 16;  // Message type for hexadecimal

    // Create or connect to the message queue
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if (msgid == -1) {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  

    // Loop to keep receiving messages
    while (running) {  
        // Receive message of type 16 (Hexadecimal)
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_receive, 0) == -1) {  
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);  
            exit(EXIT_FAILURE);  
        }  

        // Display the received hexadecimal string
        printf("Dec to Hexadecimal: %s\n", some_data.some_text);  

        // Check for termination message
        if (strncmp(some_data.some_text, "end", 3) == 0) {  
            running = 0;  
            printf("Program terminated\n");  
            break;  
        }  
    }  

    // Clean up: remove the message queue
    if (msgctl(msgid, IPC_RMID, 0) == -1) {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  

    exit(EXIT_SUCCESS);  
}
