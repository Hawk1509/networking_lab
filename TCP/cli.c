#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Needed for inet_addr()

#define PORT 2005
#define BUFFER_SIZE 100

void main() {
    int sock_desc, k;
    char buffer[BUFFER_SIZE] = {0}; // Buffer to store the message

    // Create a TCP socket (IPv4, TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server's address
    struct sockaddr_in server;
    server.sin_family = AF_INET;                   // Use IPv4 addresses
    server.sin_port = htons(PORT);                         // Port number (ideally use htons(2005))

    inet_pton(AF_INET,"127.0.0.1",&server.sin_addr);    // Convert IPv4 address from text to binary form

    // Connect the socket to the server
    k = connect(sock_desc, (struct sockaddr*)&server, sizeof(server));

    // Prompt the user for a message to send
    printf("Enter message to be sent: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // Send the message to the server
    k = send(sock_desc, buffer, BUFFER_SIZE, 0);

}
