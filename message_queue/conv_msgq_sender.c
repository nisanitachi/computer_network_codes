#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/msg.h>  

#define MAX_TEXT 512  

// Message structure to be passed through message queue
struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

int main() {
    int running = 1;
    int msgid;
    char buffer[BUFSIZ];

    // Three message structures for binary, octal, and hexadecimal results
    struct my_msg_st some_data_bin;
    struct my_msg_st some_data_oct;
    struct my_msg_st some_data_hex;

    // Generate message queue ID
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // Main loop to read input and send converted values
    while (running) {
        printf("Enter the decimal no.: ");
        fgets(buffer, BUFSIZ, stdin);

        // Assign message types for identification
        some_data_bin.my_msg_type = 2;   // For binary
        some_data_oct.my_msg_type = 8;   // For octal
        some_data_hex.my_msg_type = 16;  // For hexadecimal

        // If not "end", perform conversions
        if (strncmp(buffer, "end", 3) != 0) {
            // Convert decimal to integer
            int num = atoi(buffer);

            // ------------ Decimal to Binary ------------
            long long int bin = 0;
            int i = 1, rem;
            int temp = num;

            while (temp != 0) {
                rem = temp % 2;
                temp /= 2;
                bin += rem * i;
                i *= 10;
            }

            sprintf(some_data_bin.some_text, "%lld", bin);

            // ------------ Decimal to Octal ------------
            long long int octal = 0;
            i = 1;
            temp = num;

            while (temp != 0) {
                rem = temp % 8;
                temp /= 8;
                octal += rem * i;
                i *= 10;
            }

            sprintf(some_data_oct.some_text, "%lld", octal);

            // ------------ Decimal to Hexadecimal ------------
            char hexadecimalnum[100];
            char str[100];
            int quotient = num;
            int j = 0, k = 0;

            while (quotient != 0) {
                rem = quotient % 16;
                if (rem < 10)
                    hexadecimalnum[j++] = 48 + rem;    // ASCII 0–9
                else
                    hexadecimalnum[j++] = 55 + rem;    // ASCII A–F
                quotient /= 16;
            }

            // Reverse the string to get proper hex representation
            int len = j;
            for (k = 0; k < len; k++) {
                str[len - 1 - k] = hexadecimalnum[k];
            }
            str[len] = '\0';  // Null terminate the string

            sprintf(some_data_hex.some_text, "%s", str);
        } 
        // If input is "end", just send the word as message
        else {
            strcpy(some_data_bin.some_text, buffer);
            strcpy(some_data_oct.some_text, buffer);
            strcpy(some_data_hex.some_text, buffer);
        }

        // Send binary message
        if (msgsnd(msgid, (void *)&some_data_bin, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        // Send octal message
        if (msgsnd(msgid, (void *)&some_data_oct, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        // Send hexadecimal message
        if (msgsnd(msgid, (void *)&some_data_hex, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        // Exit if input is "end"
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    exit(EXIT_SUCCESS);
}
