#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void main() {
	int sock;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER_SIZE] = {0};
	int frame = 0, total_frames;

	// Create socket file descriptor
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation error");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		exit(EXIT_FAILURE);
	}

	// Connect to the server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Connection Failed");
		exit(EXIT_FAILURE);
	}

	printf("Enter the total number of frames to send: ");
	scanf("%d", &total_frames);

	while (frame < total_frames) {
		// Send frame
		snprintf(buffer, sizeof(buffer), "%d", frame);
		send(sock, buffer, strlen(buffer), 0);
		printf("Client: Sent frame %d\n", frame);

		// Wait for acknowledgment
		memset(buffer, 0, BUFFER_SIZE);
		int valread = read(sock, buffer, BUFFER_SIZE);
		if (valread > 0) {
			printf("Client: Received acknowledgment: %s\n", buffer);
			frame++;
		} else {
			printf("Client: No acknowledgment received. Retransmitting frame %d\n", frame);
		}
	}

	printf("All frames sent successfully.\n");
	close(sock);
}
