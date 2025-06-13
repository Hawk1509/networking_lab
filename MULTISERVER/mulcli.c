#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Needed for inet_addr()
#include <signal.h>

#define PORT 2005         // Define the port number to connect to
#define BUFFER_SIZE 1024   // Define the buffer size for messages

// Function to receive messages from the server asynchronously
void receive_messages(int sock_desc) {
    char buffer[BUFFER_SIZE];  // Buffer to store incoming messages from the server
    int bytes_received;

    // Loop to keep receiving messages as long as the connection is alive
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer before receiving a new message
        bytes_received = recv(sock_desc, buffer, BUFFER_SIZE, 0);  // Receive message

        // Print the received message from the server
        printf("Server: %s", buffer);
    }
}

void main() {
    int sock_desc, k;  // sock_desc is the socket descriptor, k is the status of socket operations
    char buffer[BUFFER_SIZE];  // Buffer to store user input

    // Create a TCP socket (IPv4, SOCK_STREAM for TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET for IPv4, SOCK_STREAM for TCP
    printf("Socket created successfully.\n");

    // Set up the server's address
    struct sockaddr_in server;  // Define server's address structure
    server.sin_family = AF_INET;  // Use IPv4 addresses
    server.sin_port = htons(PORT);  // Convert port number to network byte order (16-bit)
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address: localhost (127.0.0.1)

    // Connect to the server
    k = connect(sock_desc, (struct sockaddr*)&server, sizeof(server));  // Attempt connection
    printf("Connected to the server.\n");

    // Create a child process to handle receiving messages asynchronously
    pid_t pid = fork();  // Fork a child process

    if (pid < 0) {  // If fork fails
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        receive_messages(sock_desc);
    } else {  // Parent process
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
            printf("Enter message to be sent: ");
            fgets(buffer, BUFFER_SIZE, stdin);  // Get user input

            send(sock_desc, buffer, sizeof(buffer), 0);  // Send message to server
            }
        }
}