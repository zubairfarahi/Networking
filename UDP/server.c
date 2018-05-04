
#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Usage: ./server server_port filename
*/
int main(int argc,char**argv)
{
	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr,from_station ;
	char buffer[BUFLEN];
	int fd;
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");

	/*Open socket*/
	int socketfd;
	socketfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/*Set sockaddr_in structure to listen to that port*/
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons(atoi(argv[1]));
	my_sockaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	/* Linking socket properties */
	from_station.sin_family = AF_INET;
	from_station.sin_port = 0;
	from_station.sin_addr.s_addr = htonl(INADDR_ANY);

	int noBytesWrited;
	int bindel;
	bindel = bind(socketfd, &(my_sockaddr), sizeof(struct sockaddr));
	/* Open file for writing */
	
	int sem;
	sem = 1;

	
	
	int numberOfbytesRecived;
	int len;
	printf("areceived message\n");
	while(sem) {
		printf("received message\n");
		numberOfbytesRecived = recvfrom(socketfd, buffer, BUFLEN, 0, &from_station,(socklen_t *) &len);
		noBytesWrited = write(fd, buffer, numberOfbytesRecived);
		printf("received message\n");
		if ( noBytesWrited < 0 ) {
			perror("Erorr");
		}
		if(numberOfbytesRecived < BUFLEN) {
			sem = 0;
		}
		printf("received message\n");
	}

		

	close(socketfd);
	
	close(fd);
	return 0;
}
