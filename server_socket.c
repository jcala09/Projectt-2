#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, n;
  struct sockaddr_in serv_addr, client_addr;

  char buffer[1024];
  socklen_t len;
  // check appropriate number of CLI
  if (argc < 2) {
    printf("Enter port number <filename.c> <server_IP_addr> <portno>");
    exit(1);
  }
  portno = atoi(argv[1]);

  // create the socket IPV4 TCP
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // check if socket is made
  if (sockfd < 0) {
    printf("Oops Socket didn't open. ERROR.  Exiting code...");
    exit(1);
  }

  // build struct for server address
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  // bind socket to addr and port no. in structure. Assign IP Address

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    printf("Error in binding socket");
  // Listen to no more than  3 incoming clients at a time.
  listen(sockfd, 3);

  len = sizeof(client_addr);

  // use accept to accept coms from the client
  newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);

  // check if client socket is created properly
  if (newsockfd < 0) {
    printf("ERROR in accepting the request. Exiting now...");
    exit(1);
  }

  while (1) {
    bzero(buffer, 1024);
    // read
    n = read(newsockfd, buffer, 1024);

    if (n < 0) {
      printf("ERROR. Cannot Read. Exiting now...");
      exit(1);
    }
    // print request data
    printf("\nClient		-- > %s", buffer);
    bzero(buffer, 1024);
    // write data to client
    fgets(buffer, 1024, stdin);
    n = write(newsockfd, buffer, 1024);

    if (n < 0) {
      printf("Error in writing");
      exit(1);
    }

    // messsage goodbye closes server
    if (!strncmp("goodbye", buffer, 3)) {
      break;
    }
  }
  close(newsockfd);
  close(sockfd);
  return 0;
}