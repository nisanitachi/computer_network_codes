// receiver1.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MAX_TEXT 100

// Same message structure as sender
struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    int msgid;
    struct msg_buffer message;

    // Get access to existing message queue using same key
    msgid = msgget((key_t)1234, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Receive message of type 1 (num + 10)
    if (msgrcv(msgid, (void *)&message, sizeof(message.msg_text), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    // Display the received message
    printf("Receiver 1 received: %s\n", message.msg_text);

    return 0;
}
