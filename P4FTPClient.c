/*!
   \file P4FTPClient.c
   \brief This will be the client that will send the text files to the server
   \author Mariano Garcia
   \date 4/20/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {
    FILE *fp = NULL;

    int sockfd, port_number;
    char *filename = argv[1];
    struct sockaddr_in serv_addr;
    struct hostent *server;

  /* Will make sure there is an argument being inputted by the user */
    if (argc < 2) {
        fprintf(stderr, "No port provided");
        exit(1);
    }

      /* Gathers the user input and asigns that as the port number */
      port_number = atoi(argv[3]);

      /*first call for the socket */
      sockfd = socket(AF_INET, SOCK_STREAM, 0);

      /* error checks to make sure the socket is created successfully */
      if (sockfd <= 0) {
        perror("error opening socket");
      }

      /* gathers first user input and uses hostname */
      server = gethostbyname(argv[2]);

      /*checks to make sure the server was able to gather a hostname */
      if (server == NULL) {
        perror("No host");
        exit(1);
      }

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(port_number);

      /* error checks */
      if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
          printf("connection failure\n");
          return -1;
      }

      /* this will create a FILE and open the "test.txt" to be read */
      fp = fopen(filename, "r");
      if (fp == NULL) {
        perror("Error in reading file.");
        exit(1);
      }

      char data[256] = {0};

      /* this will run through the text file until it reaches NULL and will
      send the contents of the file to the server */
      while(fgets(data, 256, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
          perror("Error in sending file.");
          exit(1);
        }
        bzero(data, 256);
      }

      printf("File data sent successfully.\n");
      /* Closes out the socket and the FILE */
      close(sockfd);
      fclose(fp);

      return 0;
}
