

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

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server\n",file);
	exit(0);
}

int main(int argc,char**argv)
{
	if (argc!=3)
		usage(argv[0]);
	
	FILE *fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];
	
	
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	to_station.sin_family = AF_INET;
	to_station.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &to_station.sin_addr);
	
	printf("CONEXIUNE: %d\n", connect(sockfd, (struct sockaddr*)&to_station, sizeof(to_station)));

	char buffer[256];
	memset(buffer, 0, 256);

	fgets(buffer, 255, stdin);

	int n = send(sockfd, buffer, strlen(buffer), 0);
	if (n < 0){
		printf("Mesajul nu a fost transmis\n");
	}

	
	close(sockfd);
	return 0;

}
