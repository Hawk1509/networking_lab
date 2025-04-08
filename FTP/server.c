#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 5035
#define MAX 4096

int main(int argc, char *argv[]) {
    int sock_desc, client_sock_desc, client_len;
    char buffer[MAX], file_content[MAX];

    // Create socket
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server settings
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    bind(sock_desc, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("\nServer bound to port %d\n", PORT);

    // Listen for incoming connections
    listen(sock_desc, 5);
    printf("\nListening for incoming connections...\n");

    client_len = sizeof(client_addr);
    client_sock_desc = accept(sock_desc, (struct sockaddr *)&client_addr, &client_len);
    printf("\nClient connected!\n");

    // Close the main socket (not needed anymore)
    close(sock_desc);

    // Read the filename from the client
    memset(buffer, 0, MAX);
    read(client_sock_desc, buffer, MAX);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove trailing newline
    printf("\nClient requested file: %s\n", buffer);

    // Open the requested file
    FILE *file = fopen(buffer, "r");
    if (file == NULL) {
        // If file not found, send "File not found" message to the client
        strcpy(file_content, "File not found\n");
        write(client_sock_desc, file_content, strlen(file_content));
        printf("\nFile not found. Message sent to client.\n");
    } else {
        // Send file content to the client
        printf("\nTransferring file...\n");
        while (fgets(file_content, MAX, file) != NULL) {
            write(client_sock_desc, file_content, strlen(file_content));
        }
        fclose(file);
        printf("\nFile transferred successfully.\n");
    }

    // Clean up
    close(client_sock_desc);
    printf("\nServer closing.\n");

    return 0;
}
