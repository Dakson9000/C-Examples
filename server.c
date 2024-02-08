#include "op.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
/*
    File: server.c
    Author: Mason Miles
	Description: This program implements a simple server that listens for
	incoming connections, accepts client requests, processes a for an operation,
	defined in op.h, and sends a response back to the client. Usage: ./server
*/

int main() {
  // Create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  // Define the server address structure
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(11000); // Server port
  saddr.sin_addr.s_addr =
      htonl(INADDR_ANY); // Accept connections from any interface

  // Bind the socket to the server address
  if (bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) != 0) {
    perror("cannot bind");
    exit(1);
  }

  // Mark the socket as listening for incoming connections
  if (listen(sock, 0) != 0) {
    perror("Cannot listen");
    exit(2);
  }

  // Accept incoming connections in an infinite loop
  for (;;) {
    printf("Waiting for a connection \n");
    unsigned int addrlen = sizeof(struct sockaddr_in);
    int fd = accept(sock, (struct sockaddr *)&saddr, &addrlen);
    if (fd < 0) {
      perror("Cannot accept");
      exit(3);
    }

    printf("New connection accepted\n");

    // Fork a new process to handle the incoming connection
    if (fork() == 0) {
      // Process data received from the client
      struct operation op;
      recv(fd, &op, sizeof op, 0);
      op.opcode = ntohs(op.opcode);
      op.number = ntohl(op.number);
      printf("Received opcode: %d, number: %d\n", op.opcode, op.number);

      // Perform operation based on the received opcode
      if (op.opcode == INC) {
        printf("Incrementing\n");
        op.number += 1;
      } else {
        printf("Decrementing\n");
        op.number -= 1;
      }

      // Prepare response to send back to the client
      char *response;
      asprintf(&response, "%d", op.number);
      printf("Sending response: %s\n", response);
      send(fd, response, strlen(response), 0);
      free(response); // Free memory allocated for the response
      exit(0);        // Terminate the child process
    }

    // Close the connection socket in the parent process
    close(fd);
  }

  // Close the main socket
  close(sock);

  return 0;
}
