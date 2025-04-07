#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void main() {
    int sock_desc, client_sock_desc;
    char buffer[BUFFER_SIZE] = {0};
    int frame;

    // Create socket file descriptor
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the socket to the port
	struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sock_desc, (struct sockaddr *)&server, sizeof(server));

    // Listen for incoming connections
    listen(sock_desc, 3); 

    printf("Server is listening on port %d\n", PORT);

    // Accept a connection from client
	int client_len = sizeof(client);
    client_sock_desc = accept(sock_desc, (struct sockaddr *)&client, (socklen_t *)&client_len);

    printf("Connection established with client.\n");

    // Simulate receiving frames using Stop-and-Wait protocol
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(client_sock_desc, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Connection closed by client.\n");
            break;
        }

        sscanf(buffer, "%d", &frame);
        printf("Server: Received frame %d\n", frame);

        // Send acknowledgment
        char ack[BUFFER_SIZE];
        snprintf(ack, sizeof(ack), "ACK for frame %d", frame);
        send(client_sock_desc, ack, strlen(ack), 0);
        printf("Server: Sent acknowledgment for frame %d\n", frame);
    }

    // Close the sockets
    close(client_sock_desc);
    close(sock_desc);
}
