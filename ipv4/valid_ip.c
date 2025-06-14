#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if the IP address is valid and print its class and details
void checkIPAddress(const char *ip) {
    int octets[4];
    char extra; // to detect extra characters like "1.2.3.4abc"
    
    // Parse input and make sure there are exactly 4 octets, no extra characters
    if (sscanf(ip, "%d.%d.%d.%d%c", 
               &octets[0], &octets[1], &octets[2], &octets[3], &extra) != 4) {
        printf("Invalid IP address format.\n");
        return;
    }

    // Check if each octet is in the valid range [0, 255]
    for (int i = 0; i < 4; i++) {
        if (octets[i] < 0 || octets[i] > 255) {
            printf("Invalid IP address: octet %d out of range.\n", i + 1);
            return;
        }
    }

    // Now determine the class and details
    if (octets[0] >= 0 && octets[0] <= 127) {
        printf("Valid IPv4 Address.\n");
        printf("Class A\n");
        printf("Network ID: %d.0.0.0\n", octets[0]);
        printf("Broadcast ID: %d.255.255.255\n", octets[0]);
        printf("Default Mask: 255.0.0.0\n");
    } else if (octets[0] >= 128 && octets[0] <= 191) {
        printf("Valid IPv4 Address.\n");
        printf("Class B\n");
        printf("Network ID: %d.%d.0.0\n", octets[0], octets[1]);
        printf("Broadcast ID: %d.%d.255.255\n", octets[0], octets[1]);
        printf("Default Mask: 255.255.0.0\n");
    } else if (octets[0] >= 192 && octets[0] <= 223) {
        printf("Valid IPv4 Address.\n");
        printf("Class C\n");
        printf("Network ID: %d.%d.%d.0\n", octets[0], octets[1], octets[2]);
        printf("Broadcast ID: %d.%d.%d.255\n", octets[0], octets[1], octets[2]);
        printf("Default Mask: 255.255.255.0\n");
    } else if (octets[0] >= 224 && octets[0] <= 239) {
        printf("Valid IPv4 Address.\n");
        printf("Class D (Multicast Address)\n");
    } else if (octets[0] >= 240 && octets[0] <= 255) {
        printf("Valid IPv4 Address.\n");
        printf("Class E (Experimental Use)\n");
    } else {
        printf("Invalid IP address range.\n");
    }
}

int main() {
    char ip[100];
    printf("Enter an IPv4 address (x.x.x.x): ");
    scanf("%99s", ip);
    checkIPAddress(ip);
    return 0;
}
