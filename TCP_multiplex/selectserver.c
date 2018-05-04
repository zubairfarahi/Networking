#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX_CLIENTS	5
#define BUFLEN 256

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[BUFLEN];
	char anotherBuffer[BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, j;
	int k;
	int ok = 0;

	fd_set read_fds;	//the set of readings used in select ()
	fd_set tmp_fds;	//a lot used temporarily
	int fdmax;		//maximum file descriptor value in the read_fds set

	if (argc < 2) {
		fprintf(stderr, "Usage : %s port\n", argv[0]);
		exit(1);
	}

	//empty the set of read descriptors (read_fds) and the set tmp_fds
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	portno = atoi(argv[1]);

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;	// uses the IP address of the machine
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0)
		error("ERROR on binding");

	listen(sockfd, MAX_CLIENTS);

	//add the new descriptor file (socket to listen to connections) in the read_fds set
	FD_SET(sockfd, &read_fds);
	fdmax = sockfd;

	// main loop
	while (1) {
		tmp_fds = read_fds;
		if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
			error("ERROR in select");

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {

				if (i == sockfd) {
					
					clilen = sizeof(cli_addr);
					if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
						error("ERROR in accept");
					}
					else {
						//I add the new socket returned by accept () to the set of read descriptors
						FD_SET(newsockfd, &read_fds);
						if (newsockfd > fdmax) {
							fdmax = newsockfd;
						}
					}
					printf("New connection from %s, port %d, socket_client %d\n ", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), newsockfd);
				}

				else {
					// am primit date pe unul din socketii cu care vorbesc cu clientii
					//actiunea serverului: recv()
					memset(buffer, 0, BUFLEN);
					if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
						if (n == 0) {
							//conexiunea s-a inchis
							printf("selectserver: socket %d hung up\n", i);
						} else {
							error("ERROR in recv");
						}
						close(i);
						FD_CLR(i, &read_fds); // scoatem din multimea de citire socketul
					}

					else { //recv intoarce >0
						printf ("I got it from the client on the socket %d, mesg: %s", i, buffer);
						// for ()
						// if (atoi(buffer) % 2 == 0) {
						// 	printf("nu e prim\n");
						// }
						// else {
							for (k = 1; k < atoi(buffer) / 2; k++) {
								if (atoi(buffer) % i == 0) {
									ok = 1;
									// printf("prim\n");
									// printf("here\n");
									break;

									// printf("am trimis: %d\n", atoi(buffer));
								}
								if (ok == 0) {
									memset(anotherBuffer, 0, BUFLEN);
									strcpy(anotherBuffer, "prim\n");
									send(newsockfd, anotherBuffer, strlen(anotherBuffer), 0);
								}
							}
							// if (send(atoi(buffer), buffer, strlen(buffer), 0) == -1)
							// 	error("ERROR writing to socket");

							// if (atoi(buffer)) {
							// 	/* send to destination */
							// 	printf ("am trimis: %d\n", atoi(buffer));
							// 	if (send(atoi(buffer), buffer, strlen(buffer), 0) == -1)
							// 		error("ERROR writing to socket");
							// }

							// else {
							// 	printf("mesaj tuturor\n");
							// 	for (j = 0; j <= fdmax; j++) {
							// 		if (FD_ISSET(j, &read_fds)) {
							// 			if (j != sockfd && j != i) {
							// 				if (send(j, buffer, strlen(buffer), 0) == -1)
							// 					error("ERROR writing to socket");
							// 			}
							// 		}
							// 	}
							// }
						}
					}
				}
			}
		}


		close(sockfd);

		return 0;
	}

