#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define PORT 2005
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS] = {0};  // Stores connected clients

// Function to broadcast message to all clients except the sender
void broadcast_message(char *message, int sender_sock) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_sock) {
            send(client_sockets[i], message, sizeof(message), 0);
        }
    }
}

// Handle client communication
void handle_client(int client_sock_desc) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(client_sock_desc, buffer, BUFFER_SIZE, 0);

        printf("Client: %s\n", buffer);
        broadcast_message(buffer, client_sock_desc);
    }

    // Remove client from the list
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_sock_desc) {
            client_sockets[i] = 0;
            break;
        }
    }

}

void main() {
    int sock_desc, client_sock_desc; 

    // Create a TCP socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    printf("Socket created successfully.\n");

    // Set up the server address structure
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;          // IPv4 address family
    server_addr.sin_port = htons(PORT);        // Port number (2005)
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP

    socklen_t client_len = sizeof(client_addr);

    // Bind the socket to the address and port specified in server_addr
    bind(sock_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Listen for incoming connections; allow up to 10 pending connections
    listen(sock_desc, MAX_CLIENTS);
    printf("Listening for connections on port %d...\n", PORT);

    // Accept multiple clients
    while (1) {
    client_sock_desc = accept(sock_desc, (struct sockaddr*)&client_addr, &client_len);
    printf("New client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == 0) {
            // Found an available slot, add client socket
            client_sockets[i] = client_sock_desc;
            break;
        }
    }

    if (i == MAX_CLIENTS) {
        // No available slot, reject the client
        printf("Server is full! Rejecting client.\n");
        close(client_sock_desc);
    } else if (fork() == 0) { // Child process
        // Close the server socket in the child process
        //close(sock_desc);
        handle_client(client_sock_desc);
    }
}

    }
