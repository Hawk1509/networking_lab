#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 2005
#define BUFFER_SIZE 100
#define SA struct sockaddr

struct timeval timeout;

void send_frame(int sock_desc, int frame) {
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%d", frame);  // Directly convert the frame number to a string
    send(sock_desc, buffer, strlen(buffer) + 1, 0);  // Send the string frame
    printf("Frame %d sent\n", frame);
}


void resend_frames(int sock_desc, int start, int nf, int ws) {
    char buffer[BUFFER_SIZE];
    for (int j = start; j < nf && j < start + ws; j++) {
        sprintf(buffer, "%d", j);  // Directly convert the frame number to a string
        send(sock_desc, buffer, strlen(buffer) + 1, 0);  // Send only the used part of the buffer
        printf("Frame %d resent\n", j);
    }
}


void func(int sock_desc, int nf, int ws) {
    char buffer[BUFFER_SIZE];
    int ack, i = 0, n;
    int w1 = 0, w2 = ws - 1;

    // Set socket timeout
    setsockopt(sock_desc, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // Send initial window of frames
    for (i = 0; i <= w2 && i < nf; i++) {
        send_frame(sock_desc, i);
    }

    while (1) {
        // Send new frames if the window allows
        if (w2 - w1 < ws - 1 && i < nf) {
            send_frame(sock_desc, i);
            w2++;
            i++;
        }

        n = recv(sock_desc, buffer, BUFFER_SIZE, 0);
        if (n > 0) {
            ack = atoi(buffer);  // Convert received ack to integer

            // Check if we received the last acknowledgment
            if (ack + 1 == nf) {
                printf("Acknowledgement received: %d\nExit\n", ack);
                send(sock_desc, "Exit", sizeof("Exit"), 0);  // Exit signal to server
                break;
            }

            // Handle valid acknowledgment
            if (ack == w1) {
                w1++;  // Slide window
                printf("Acknowledgement received: %d\n", ack);
            }
        } else {
            // No acknowledgment received, resend from w1
            printf("Acknowledgement not received for %d\nResending frames\n", w1);
            resend_frames(sock_desc, w1, nf, ws);
        }
    }
}


void main() {
    int sock_desc, k;
    int num_frames, window_size;

    // Create a TCP socket (IPv4, TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket successfully created\n");

    // Initialize server address
    struct sockaddr_in server;
    //bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // Use the same port as in the TCP client code
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert the server address to binary form

    // Set timeout for socket operations
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    // Connect the socket to the server
    k = connect(sock_desc, (SA *)&server, sizeof(server));
    printf("Connected to the server\n");

    // Get user input for number of frames and window size
    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    // Start Go-Back-N protocol
    func(sock_desc, num_frames, window_size);

    // Close the socket when done
}
