#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main() {
    int sock_desc, k;        // sock_desc: socket descriptor; k: variable for function return values
    char buf[100];           // Buffer to hold the message (up to 100 characters)

    // Create a UDP socket:
    // AF_INET indicates IPv4; SOCK_DGRAM indicates UDP protocol.
    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);

    // Define the server's address:
    struct sockaddr_in server;
    server.sin_family = AF_INET;                    // Set address family to IPv4
    server.sin_port = 2005;                          // Set the server port to 2005 (ideally use htons for conversion)
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // Set server IP to localhost (127.0.0.1)

    // Prompt the user for a message:
    printf("Enter the msg to be sent: ");
    fgets(buf, 100, stdin);  // Read up to 100 characters from standard input into buf

    // Send the message via UDP:
    // sendto sends the message in buf to the specified server address.
    k = sendto(sock_desc, buf, 100, 0, (struct sockaddr*)&server, sizeof(server));
}
