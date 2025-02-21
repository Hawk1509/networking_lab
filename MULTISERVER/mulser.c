#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 2005
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS] = {0};  // Stores connected clients

// Function to broadcast message to all clients except the sender
void broadcast_message(char *message, int sender_sock) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_sock) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
}

// Handle client communication
void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client: %s\n", buffer);
        broadcast_message(buffer, client_sock);
    }

    // Remove client from the list
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_sock) {
            client_sockets[i] = 0;
            break;
        }
    }

    close(client_sock);
    exit(0); // Terminate child process
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Ignore SIGCHLD to prevent zombie processes
    signal(SIGCHLD, SIG_IGN);

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Bind successful.\n");

    // Listen for connections
    if (listen(server_sock, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Listening for connections on port %d...\n", PORT);

    // Accept multiple clients
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }
        printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Add client to the list
        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_sock;
                break;
            }
        }

        if (i == MAX_CLIENTS) {
            printf("Server is full! Rejecting client.\n");
            close(client_sock);
        } else {
            // Fork a new process for each client
            pid_t pid = fork();
            if (pid == 0) { // Child process
                close(server_sock); // Child does not need the server socket
                handle_client(client_sock);
            } else if (pid < 0) {
                perror("Fork failed");
            }
        }
    }

    close(server_sock);
    return 0;
}

