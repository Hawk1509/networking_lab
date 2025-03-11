#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Needed for inet_addr()

void main() {
    int sock_desc, k;
    char buf[100];

    // Create a TCP socket (IPv4, TCP)
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server's address
    struct sockaddr_in server;
    server.sin_family = AF_INET;                   // Use IPv4 addresses
    server.sin_port = 2005;                         // Port number (ideally use htons(2005))
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP: localhost

    // Connect the socket to the server
    k = connect(sock_desc, (struct sockaddr*)&server, sizeof(server));

    // Prompt the user for a message to send
    printf("Enter message to be sent: ");
    fgets(buf, 100, stdin);

    // Send the message to the server
    k = send(sock_desc, buf, 100, 0);

}
