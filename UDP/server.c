
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 2005
#define BUFFER_SIZE 100

void main()
{
    // Declare variables: 
    // sock_desc is the socket descriptor for the server,
    // client_sock_desc is declared but not used correctly for UDP.
    int sock_desc, client_sock_desc;
    char buffer[BUFFER_SIZE] = {0};  // Buffer to store the incoming message

    // Create a UDP socket using IPv4
    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);

    // Define server and client address structures
    struct sockaddr_in server, client;
    server.sin_family = AF_INET;          // Use IPv4 address family
    server.sin_port = htons(PORT);                // Set the port number to 2005
    server.sin_addr.s_addr = INADDR_ANY;   // Accept connections on any local IP address

    // Bind the socket to the specified server address and port
    bind(sock_desc, (struct sockaddr*)&server, sizeof(server));

    // Set the length of the client address structure
    socklen_t client_len = sizeof(client);

    // Receive a message from any client (UDP is connectionless)
    recvfrom(sock_desc, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client, &client_len);

    // Print that a message has been received
    printf("Connection established");

    // NOTE: This recv() call uses client_sock_desc, which is uninitialized.
    // For UDP, recvfrom() is sufficient, so this line may not work as expected.


    // Print the message received from the client
    printf("\nMessage from client: %s\n", buffer);
}
