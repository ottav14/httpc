#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void initialize_connection(int* sock) {

	int opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
	
    // Create socket file descriptor
    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

	// Set socket options to reuse address and port
    if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
		close(*sock);
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to the port
    if (bind(*sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(*sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(*sock, 3) < 0) {
        perror("Listen failed");
        close(*sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept a client connection
    if ((*sock = accept(*sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(*sock);
        exit(EXIT_FAILURE);
    }
}

int main() {

    int sock = 0;
    const char* message = "test message";

	initialize_connection(&sock);

	// Send response to client
	send(sock, message, strlen(message), 0);
	printf("Data transmitted.\n");

    close(sock);
	printf("Process terminated.\n");
    return 0;
}

