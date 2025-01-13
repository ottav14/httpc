#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int initialize_connection(int* sock) {

    struct sockaddr_in serv_addr;

    // Create socket
    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4/IPv6 address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(*sock);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(*sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(*sock);
        exit(EXIT_FAILURE);
    }
}

int main() {

	int sock = 0;
    const char *message = "Hello from client";
    char buffer[1024] = {0};

	initialize_connection(&sock);
	printf("Connection established.\n");

	// Read response from server
	read(sock, buffer, sizeof(buffer));
	printf("Response from server: %s\n", buffer);

    close(sock);
    return 0;
}

