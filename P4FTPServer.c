/*!
   \file server.c
   \brief The server file
   \author Mariano Garcia
   \date 4/20/20
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char const *argv[]) {

    FILE *fp = NULL;

    int sockfd, newsockfd, port_number, client, v_read;
    char buffer[256];
    char *filename = "recv.txt";
    struct sockaddr_in serv_addr;

    /* error checks to make sure there was user input */
    if (argc < 2) {
        fprintf(stderr, "No port provided");
        exit(1);
    }

    /*creates the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* error checks to make sure socket was created */
    if (sockfd <= 0) {
        perror("error opening socket");
    }

    /*Gathers the port number from user input */
    port_number = atoi(argv[1]);

    /* Needed to create the server connections */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);

    /* error checks */
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("bind failure");
      exit(0);
    }

    listen(sockfd, 5);

    if ((newsockfd = accept(sockfd, (struct sockaddr *)&serv_addr, (socklen_t*)&client)) < 0) {
        perror("Accept error");
        exit(0);
    }

    /* opens a new file that we will export the contexts of the text file from
    client */

    fp = fopen(filename, "w");
    /*will read out the contexts of the text file and print it onto the new
    text file*/
    while (1) {
        v_read = read(newsockfd, buffer, 1024);
        if (v_read <= 0){
            break;
        }
        fprintf(fp, "%s", buffer);
      }

    printf("test.txt has: %s\n", buffer);

  return 0;
}
