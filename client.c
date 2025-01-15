#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void initialize_connection(int* sock) {

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
    char buffer[BUFFER_SIZE] = {0};

	initialize_connection(&sock);
	printf("Connection established.\n\n");

	while(1) {
		char* request = NULL;
		size_t len = 0;
		printf("Enter request:\n");
		getline(&request, &len, stdin);
		int cr_ix = 0;
		while(request[cr_ix] != '\n')
			cr_ix++;
		request[cr_ix] = 0;
		send(sock, request, sizeof(request), 0);
		printf("\nRequest transmitted.\n\n");
		if(strcmp(request, "exit") == 0)
			break;
	}

    close(sock);
    return 0;
}

