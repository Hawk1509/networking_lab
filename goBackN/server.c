#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>  // Needed for inet_addr()

#define MAX 80
#define PORT 2005  // Port updated to match the TCP server port
#define BUFFER_SIZE 100  // Buffer size updated to match the TCP server

void handle_client(int client_sock_desc) {
    char buffer[BUFFER_SIZE];
    int frame, ack = 0, next_frame = 0;

    while (1) {
        sleep(1);
        bzero(buffer, sizeof(buffer));

        // Receive data from the client
        recv(client_sock_desc, buffer, BUFFER_SIZE, 0);

        // Check for exit condition
        if (strcmp("Exit", buffer) == 0) {
            printf("Exit\n");
            break;
        }

        frame = atoi(buffer);

        // Handle out-of-order frames
        if (frame != next_frame) {
            printf("Frame %d discarded\nAcknowledgement sent: %d\n", frame, ack);
            bzero(buffer, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%d", ack);
            send(client_sock_desc, buffer, sizeof(buffer), 0);
            continue;
        }

        // Simulate random behavior for frame reception
        int random_behavior = rand() % 3;
        switch (random_behavior) {
            case 0:
                // Frame not received (simulated)
                break;

            case 1:
            case 2:
                // Frame received
                ack = frame;
                printf("Frame %d received\nAcknowledgement sent: %d\n", frame, ack);

                bzero(buffer, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "%d", ack);
                send(client_sock_desc, buffer, sizeof(buffer), 0);

                next_frame = ack + 1;
                break;
        }
    }
}

void main() {
    int sock_desc, client_sock_desc;
    struct sockaddr_in server, client;
    socklen_t client_len;
    char buffer[BUFFER_SIZE] = {0};  // Buffer to store received data

    // Create a TCP socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created\n");

    // Set up the server address structure
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;             // IPv4 address family
    server.sin_port = htons(PORT);           // Port number (2005)
    server.sin_addr.s_addr = INADDR_ANY;     // Accept connections from any IP address

    // Bind the socket to the address and port
    if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0) {
        perror("Socket bind failed");
        close(sock_desc);
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully binded\n");

    // Listen for incoming connections; allow up to 5 pending connections
    if (listen(sock_desc, 5) != 0) {
        perror("Listen failed");
        close(sock_desc);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    client_len = sizeof(client);

    // Accept a connection and get a new socket descriptor for communication with the client
    client_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &client_len);
    if (client_sock_desc < 0) {
        perror("Server accept failed");
        close(sock_desc);
        exit(EXIT_FAILURE);
    }
    printf("Server accepted the client\n");

    // Handle client communication using Go-Back-N protocol
    handle_client(client_sock_desc);

    // Close the socket
    close(client_sock_desc);
    close(sock_desc);
}
