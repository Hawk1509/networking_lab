#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void process_client(int client_sock_desc) {
    char buff[MAX];
    int frame, ack, next = 0;
    int *received = (int *)calloc(MAX, sizeof(int)); // Array to track received frames

    while (1) {
        //bzero(buff, MAX);
        recv(client_sock_desc, buff, MAX, 0);

        if (strcmp("Exit", buff) == 0) {
            printf("Exit\n");
            break;
        }

        frame = atoi(buff);
        if (frame < next || frame >= next + MAX) {
            // Out-of-order frame, send NACK
            printf("Frame %d discarded\nNACK sent: %d\n", frame, next - 1);
            snprintf(buff, sizeof(buff), "%d", -1); // Send NACK
            send(client_sock_desc, buff, sizeof(buff), 0);
            continue;
        }

        // Simulate random packet loss or delays
        ack = rand() % 3;
        switch (ack) {
            case 0: // Frame not received, send NACK
                printf("Frame %d not received\nNACK sent: %d\n", frame, next - 1);
                snprintf(buff, sizeof(buff), "%d", -1); // Send NACK
                send(client_sock_desc, buff, sizeof(buff), 0);
                break;
            case 1: // Frame received with delay
            case 2: // Frame received correctly
                received[frame] = 1;
                printf("Frame %d received\nAcknowledgment sent: %d\n", frame, frame);
                snprintf(buff, sizeof(buff), "%d", frame);
                send(client_sock_desc, buff, sizeof(buff), 0);
                break;
        }
    }

    free(received);
}

void main() {
    int sock_desc, client_sock_desc, client_len;

    // Create the server socket
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    //bzero(&server, sizeof(server));
    
    struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind the socket to the server's address
    bind(sock_desc, (SA *)&server, sizeof(server));

    // Listen for incoming connections
    listen(sock_desc, 5);

    printf("Server listening on port %d...\n", PORT);

    // Accept an incoming client connection
    client_len = sizeof(client);
    client_sock_desc = accept(sock_desc, (SA *)&client, &client_len);

    printf("Connection established with client\n");

    // Process the client
    process_client(client_sock_desc);


}
