#include "helpers.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define len 1000

void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_ip server_port\n",file);
	exit(0);
}

int main(int argc,char**argv)
{
	if (argc != 3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, client_addr;
	char buffer[BUFLEN];
	int portno;
	int sockfd2; 

	sscanf(argv[2], "%d", &portno);

	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		printf("can't open socket");
	}

    my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;
	my_sockaddr.sin_port = htons(portno);
	
    bind(sockfd, (struct sockaddr *) &my_sockaddr, sizeof(struct sockaddr));

    listen (sockfd, len);

    int addrlen = sizeof (struct sockaddr_in);

    sockfd2 = accept (sockfd, (struct  sockaddr*) &client_addr, &addrlen);

    if (sockfd2 < 0) {
    	printf("can't accept socket");
    }

    while (1) {
    	memset (buffer, 0, 256);
    	int n = recv (sockfd2, buffer, BUFLEN, 0);

    	if (n != -1) {
            printf ("%s\n", buffer);
            break;
        }
    } 

	
    close (sockfd);
    close (sockfd2);

	return 0;
}
