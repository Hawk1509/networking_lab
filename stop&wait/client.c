#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void main() {
    int sock_desc, valread;
    char buf[BUFFER_SIZE] = {0};
    int frame = 0, total_frames;

    // Create socket file descriptor
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    // Set up the server address
    struct sockaddr_in server;
	server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // Convert to network byte order

    // Convert IPv4 address from text to binary form
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    // Connect to the server
    connect(sock_desc, (struct sockaddr *)&server, sizeof(server));
    
    printf("Enter the total number of frames to send: ");
    scanf("%d", &total_frames);

    while (frame < total_frames) {
        // Send frame
        snprintf(buf, sizeof(buf), "%d", frame);  // Convert the frame number to a string
        send(sock_desc, buf, strlen(buf), 0);     // Send the frame to the server
        printf("Client: Sent frame %d\n", frame);

        // Wait for acknowledgment from the server
        memset(buf, 0, BUFFER_SIZE);  // Clear the buffer
        valread = read(sock_desc, buf, BUFFER_SIZE);
        if (valread > 0) {
            printf("Client: Received acknowledgment: %s\n", buf);
            frame++;  // Move to the next frame after receiving acknowledgment
        } else {
            printf("Client: No acknowledgment received. Retransmitting frame %d\n", frame);
        }
    }

    printf("All frames sent successfully.\n");
    close(sock_desc);  // Close the socket after all frames are sent
}
