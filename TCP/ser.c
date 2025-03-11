#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>

void main()
{
    int sock_desc, client_sock_desc;
    char buf[100];

    // Create a TCP socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;          // IPv4 address family
    server_addr.sin_port = 2005;                // Port number (2005)
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Accept connections from any IP

    // Bind the socket to the address and port specified in server_addr
    bind(sock_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen for incoming connections; allow up to 3 pending connections
    listen(sock_desc, 3);

    // Prepare to accept a connection from a client
    struct sockaddr_in client_addr;
    socklen_t client_len;
    client_len = sizeof(client_addr);
    
    // Accept a connection and get a new socket descriptor for communication with the client
    client_sock_desc = accept(sock_desc, (struct sockaddr*)&client_addr, &client_len);

    // Connection is established if accept() is successful
    printf("Connection established");

    // Receive data from the client and store it in buf (up to 100 bytes)
    int bytes_received = recv(client_sock_desc, buf, 100, 0);

    // Print the message received from the client
    printf("Message from client: %s\n", buf);
}
