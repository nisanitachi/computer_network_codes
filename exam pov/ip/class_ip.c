#include <stdio.h>
#include <stdlib.h>

void checkIPClass(const char *ip) {
    int octet1;

    // Read the first octet using sscanf
    if (sscanf(ip, "%d", &octet1) != 1 || octet1 < 0 || octet1 > 255) {
        printf("Invalid IP address format or first octet out of range.\n");
        return;
    }

    // Determine the class based on the first octet
    if (octet1 >= 0 && octet1 <= 127) {
        printf("Class A\n");
    } else if (octet1 >= 128 && octet1 <= 191) {
        printf("Class B\n");
    } else if (octet1 >= 192 && octet1 <= 223) {
        printf("Class C\n");
    } else if (octet1 >= 224 && octet1 <= 239) {
        printf("Class D (Multicast)\n");
    } else if (octet1 >= 240 && octet1 <= 255) {
        printf("Class E (Experimental)\n");
    } else {
        printf("Invalid first octet value.\n");
    }
}

int main() {
    char ip[20];

    printf("Enter an IPv4 address (x.x.x.x): ");
    scanf("%19s", ip);

    checkIPClass(ip);

    return 0;
}
