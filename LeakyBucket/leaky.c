#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main() {
    int b, n, r, d, lvl = 0;
    printf("Enter the number of rounds: ");
    scanf("%d", &n);

    int pkt[n]; // Array to hold the number of packets for each round

    printf("Enter the Bucket size: ");
    scanf("%d", &b);

    printf("Enter the outgoing rate: ");
    scanf("%d", &r);

    // Loop through each round
    for (int i = 0; i < n; i++) {
        printf("\n\nEnter the number of packets to be sent in Round %d: ", i + 1);
        scanf("%d", &pkt[i]);
        printf("[INCOMING] %d packets\n", pkt[i]);

        // Calculate the dropped packets
        d = (pkt[i] + lvl) - b;
        if (d > 0) {
            printf("[DROPPED] %d packets\n", d);
            lvl = (pkt[i] + lvl) - d; // Update bucket level after dropping packets
        } else {
            lvl = (pkt[i] + lvl); // No drop, just add incoming packets to the bucket
        }

        printf("Current number of packets in bucket: %d\n", lvl);

        // Handle outgoing packets based on the rate
        if (lvl - r > 0) {
            printf("[OUTGOING] %d packets\n", r);
            lvl -= r; // Reduce the bucket level by the outgoing rate
        } else {
            printf("[OUTGOING] %d packets\n", lvl);
            lvl = 0; // If less than the outgoing rate, empty the bucket
        }

        printf("Number of packets left in the bucket: %d\n", lvl);
    }

    // If there are still packets left in the bucket after all rounds, send them
    while (lvl > 0) {
        if (lvl - r > 0) {
            printf("[OUTGOING] %d packets\n", r);
            lvl -= r; // Continue sending packets until bucket is empty
        } else {
            printf("[OUTGOING] %d packets\n", lvl);
            lvl = 0; // Empty the bucket
        }
        printf("Number of packets left in the bucket: %d\n", lvl);
    }

    printf("Bucket empty\n");
}
