

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Usage: ./client ip_server port_server file_file_name
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];


	/*Open socket*/
	
	
	/* Open file for reading */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Set struct sockaddr_in to specify where to send data*/
	
	
	


	return 0;
}