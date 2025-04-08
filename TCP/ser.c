#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>

#define PORT 2005
#define BUFFER_SIZE 100

void main()
{
    int sock_desc, client_sock_desc;
    char buffer[BUFFER_SIZE] = {0}; // Buffer to store received data

    // Create a TCP socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    struct sockaddr_in server, client;
    server.sin_family = AF_INET;          // IPv4 address family
    server.sin_port = htons(PORT);                // Port number (2005)
    server.sin_addr.s_addr = INADDR_ANY;   // Accept connections from any IP

    // Bind the socket to the address and port specified in server_addr
    bind(sock_desc, (struct sockaddr*)&server, sizeof(server));

    // Listen for incoming connections; allow up to 3 pending connections
    listen(sock_desc, 3);

    // Prepare to accept a connection from a client
    socklen_t client_len;
    client_len = sizeof(client);
    
    // Accept a connection and get a new socket descriptor for communication with the client
    client_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &client_len);

    // Connection is established if accept() is successful
    printf("Connection established\n");

    // Receive data from the client and store it in buf (up to 100 bytes)
    int bytes_received = recv(client_sock_desc, buffer, BUFFER_SIZE, 0);

    // Print the message received from the client
    printf("Message from client: %s\n", buffer);
}
