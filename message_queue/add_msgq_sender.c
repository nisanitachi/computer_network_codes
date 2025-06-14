// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MAX_TEXT 100

// Structure for message queue
struct msg_buffer {
    long msg_type;           // Message type (used for selective receiving)
    char msg_text[MAX_TEXT]; // Message content
};

int main() {
    int msgid;
    struct msg_buffer message;

    // Create a message queue (or get if already exists)
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT); // 0666 = RW permissions
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    int num;
    printf("Enter a decimal number: ");
    scanf("%d", &num);

    // Send (num + 10) with type 1
    message.msg_type = 1;
    sprintf(message.msg_text, "%d", num + 10);
    msgsnd(msgid, (void *)&message, sizeof(message.msg_text), 0);

    // Send (num + 20) with type 2
    message.msg_type = 2;
    sprintf(message.msg_text, "%d", num + 20);
    msgsnd(msgid, (void *)&message, sizeof(message.msg_text), 0);

    printf("Messages sent to queue.\n");

    return 0;
}
