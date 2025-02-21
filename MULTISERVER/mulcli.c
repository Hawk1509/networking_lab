#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2005
#define BUFFER_SIZE 1024

void receive_messages(int sock_desc) {
    char buffer[BUFFER_SIZE];
    int bytes_received;
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(sock_desc, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Disconnected from server.\n");
            close(sock_desc);
            exit(0);
        }
        printf("Server: %s", buffer);
    }
}

int main() {
    int sock_desc;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // Convert to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sock_desc, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Create a child process to receive messages asynchronously
    pid_t pid = fork();
    if (pid == 0) {  
        // Child process: Receives messages
        receive_messages(sock_desc);
    } else if (pid > 0) {  
        // Parent process: Sends messages
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            send(sock_desc, buffer, strlen(buffer), 0);
        }
    } else {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    close(sock_desc);
    return 0;
}

