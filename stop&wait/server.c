#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
int server_fd, new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {0};
int frame;

// Create socket file descriptor
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
perror("Socket failed");
exit(EXIT_FAILURE);
}

// Bind the socket to the port
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);

if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
perror("Bind failed");

exit(EXIT_FAILURE);
}

// Listen for incoming connections
if (listen(server_fd, 3) < 0) {
perror("Listen failed");
exit(EXIT_FAILURE);
}

printf("Server is listening on port %d\n", PORT);

// Accept a connection
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
perror("Accept failed");
exit(EXIT_FAILURE);
}

printf("Connection established with client.\n");

// Simulate receiving frames using Stop-and-Wait protocol
while (1) {
memset(buffer, 0, BUFFER_SIZE);
int valread = read(new_socket, buffer, BUFFER_SIZE);
if (valread <= 0) {
printf("Connection closed by client.\n");
break;
}

sscanf(buffer, "%d", &frame);
printf("Server: Received frame %d\n", frame);

// Send acknowledgment
char ack[BUFFER_SIZE];
snprintf(ack, sizeof(ack), "ACK for frame %d", frame);
send(new_socket, ack, strlen(ack), 0);
printf("Server: Sent acknowledgment for frame %d\n", frame);
}

close(new_socket);
close(server_fd);
return 0;
}