#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 100
#define SA struct sockaddr

struct timeval timeout;

void send_frames(int sock_desc, int num_frames, int window_size) {
    char buffer[BUFFER_SIZE];
    int ack, i = 0, k, w1 = 0, w2 = window_size - 1;
    int *acked = (int *)calloc(num_frames, sizeof(int));

    // Set socket receive timeout
    setsockopt(sock_desc, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Initial frame sending
    for (i = 0; i < num_frames && i <= w2; i++) {
        snprintf(buffer, sizeof(buffer), "%d", i);
        send(sock_desc, buffer, sizeof(buffer), 0);
        printf("Frame %d sent\n", i);
    }

    while (1) {
        if (w2 - w1 < window_size - 1 && i < num_frames) {
            snprintf(buffer, sizeof(buffer), "%d", i);
            send(sock_desc, buffer, sizeof(buffer), 0);
            printf("Frame %d sent\n", i);
            w2++;
            i++;
        }

        // Receive acknowledgment
        bzero(buffer, sizeof(buffer));
        int n = recv(sock_desc, buffer, BUFFER_SIZE, 0);
        if (n > 0) {
            ack = atoi(buffer);
            if (ack == -1) {
                printf("NACK received for frame %d\n", w1);
                snprintf(buffer, sizeof(buffer), "%d", w1);
                send(sock_desc, buffer, sizeof(buffer), 0);
            } else {
                acked[ack] = 1;
                printf("Acknowledgment received for frame %d\n", ack);
                while (acked[w1] && w1 < num_frames) {
                    w1++;
                }
            }

            if (ack + 1 == num_frames) {
                snprintf(buffer, sizeof(buffer), "Exit");
                send(sock_desc, buffer, sizeof(buffer), 0);
                break;
            }
        } else {
            printf("Timeout, resending frame %d\n", w1);
            snprintf(buffer, sizeof(buffer), "%d", w1);
            send(sock_desc, buffer, sizeof(buffer), 0);
        }
    }

    free(acked);
}

void main() {
    int sock_desc, num_frames, window_size;
    struct sockaddr_in server;
    
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    connect(sock_desc, (SA *)&server, sizeof(server));

    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    send_frames(sock_desc, num_frames, window_size);

}
