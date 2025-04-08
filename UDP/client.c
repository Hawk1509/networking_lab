#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2005
#define BUFFER_SIZE 100

void main() {
    int sock_desc, k;        // sock_desc: socket descriptor; k: variable for function return values
    char buffer[BUFFER_SIZE] = {0};           // Buffer to hold the message (up to 100 characters)

    // Create a UDP socket:
    // AF_INET indicates IPv4; SOCK_DGRAM indicates UDP protocol.
    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);

    // Define the server's address:
    struct sockaddr_in server;
    server.sin_family = AF_INET;                    // Set address family to IPv4
    server.sin_port = htons(PORT);                          // Set the server port to 2005 (ideally use htons for conversion)

    inet_pton(AF_INET,"127.0.0.1", &server.sin_addr);

    // Prompt the user for a message:
    printf("Enter the msg to be sent: ");
    fgets(buffer, BUFFER_SIZE, stdin);  // Read up to 100 characters from standard input into buf

    // Send the message via UDP:
    // sendto sends the message in buf to the specified server address.
    k = sendto(sock_desc, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server, sizeof(server));
}
