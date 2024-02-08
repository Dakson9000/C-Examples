#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "op.h"
/*
    File: client.c
    Author: Mason Miles
	Description: This program implements a simple client that connects to a server and
	sends a request for an operation, defined in op.h, and receives a response from the
	server. 
	Usage: ./client hostname
*/
int main(int argc, char* argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(1);
    }

    // Convert hostname to IP address
    char* hostname = argv[1];
    struct hostent* host = gethostbyname(hostname);
    struct in_addr* addr = (struct in_addr*) host->h_addr;

    // Convert internet address to a string for verification
    char* hoststr = inet_ntoa(*addr);
    printf("The address of %s is %s\n", hostname, hoststr);

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Connect the socket to the server
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(11000); // Server port
    saddr.sin_addr.s_addr = addr->s_addr;

    // Attempt to establish a connection
    if (connect(sock, (struct sockaddr*) &saddr, sizeof(struct sockaddr_in)) != 0) {
        perror("Couldn't connect");
        exit(1);
    }

    // Prepare data to send
    struct operation op;
    op.opcode = htons(INC); // Opcode for increment operation
    op.number = htonl(random() % 100); // Random number to operate on
    printf("Sending opcode: %d, number: %d\n", ntohs(op.opcode), ntohl(op.number));
    write(sock, &op, sizeof(op));

    // Read response from the server
    char buf[8192];
    memset(buf, 0, sizeof(buf));
    read(sock, buf, sizeof(buf));
    
    // Print the response received from the server
    printf("%s\n", buf);

    // Close the socket
    close(sock);

    return 0;
}
