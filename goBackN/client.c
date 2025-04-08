#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX 80
#define PORT 2005          // Port number updated to match the TCP client code
#define BUFFER_SIZE 100    // Buffer size updated to match the TCP client code
#define SA struct sockaddr

struct timeval timeout;

void send_frame(int sock_desc, int frame) {
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%d", frame);
    send(sock_desc, buffer, sizeof(buffer), 0);
    printf("Frame %d sent\n", frame);
}

void resend_frames(int sock_desc, int start, int nf, int ws) {
    char buffer[BUFFER_SIZE];
    for (int j = start; j < nf && j < start + ws; j++) {
        bzero(buffer, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%d", j);
        send(sock_desc, buffer, sizeof(buffer), 0);
        printf("Frame %d sent\n", j);
    }
}

void func(int sock_desc, int nf, int ws) {
    char buffer[BUFFER_SIZE];
    int ack, i = 0, n;
    int w1 = 0, w2 = ws - 1;
    int flag = 0;

    // Set socket timeout
    if (setsockopt(sock_desc, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt(SO_RCVTIMEO) failed");
    }

    // Send initial window of frames
    for (i = 0; i < nf && i <= w2; i++) {
        send_frame(sock_desc, i);
    }

    while (1) {
        // Send new frames if window allows
        if (w2 - w1 != ws - 1 && flag == 0 && i != nf) {
            send_frame(sock_desc, i);
            w2++;
            i++;
        }

        flag = 0;
        bzero(buffer, sizeof(buffer));
        n = recv(sock_desc, buffer, BUFFER_SIZE, 0);
        ack = atoi(buffer);

        if (n > 0) {
            // Acknowledgement received
            if (ack + 1 == nf) {
                printf("Acknowledgement received: %d\nExit\n", ack);
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "Exit");
                send(sock_desc, buffer, sizeof(buffer), 0);
                break;
            }
            if (ack == w1) {
                w1++;
                printf("Acknowledgement received: %d\n", ack);
            }
        } else {
            // Acknowledgement not received, resend frames
            printf("Acknowledgement not received for %d\nResending frames\n", w1);
            resend_frames(sock_desc, w1, nf, ws);
            flag = 1;
        }
    }
}

void main() {
    int sock_desc, k;
    int num_frames, window_size;
    char buffer[BUFFER_SIZE] = {0}; // Buffer to store user input

    // Create a TCP socket (IPv4, TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created\n");

    // Initialize server address
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // Use the same port as in the TCP client code

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert the server address to binary form

    // Set timeout for socket operations
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    // Connect the socket to the server
    k = connect(sock_desc, (SA *)&server, sizeof(server));
    if (k == -1) {
        perror("Connection with the server failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server\n");

    // Get user input for number of frames and window size
    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    // Start Go-Back-N protocol
    func(sock_desc, num_frames, window_size);

    // Close the socket when done
    close(sock_desc);
}
