#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;

  char buffer[1024];
  // check for appropriate CLI

  if (argc < 3) {
    printf("Enter port number <filename.c> <server_IP_addr> <portno>");
    exit(1);
  }
  portno = atoi(argv[2]);

  // create the socket IPV4 TCP
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("ERROR opening socket. Exiting now...");
    exit(1);
  }
  // build struct for server address
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htons(portno);
  serv_addr.sin_port = inet_addr(argv[1]);

  // Connect to server
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Connection failed. Exiting now...");
    exit(1);
  }

  while (1) {
    bzero(buffer, 1024); // clear buffer
    // now write to server
    fgets(buffer, 1024, stdin);
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
      printf("error in writing. exiting now...");
      exit(1);
    }
    // read back from server
    bzero(buffer, 1024); // clear buffer
    n = read(sockfd, buffer, 1024);

    if (n < 0) {
      printf("error in reading. exiting now...");
      exit(1);
    }
    // print server's response
    printf("\nServer --> %s", buffer);
    // detect end of convo
    if (!strncmp("goodbye", buffer, 3))
      break;
  }
  close(sockfd);
  return 0;
}