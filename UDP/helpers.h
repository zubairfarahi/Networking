#ifndef _HELPERS_H
#define _HELPERS_H 1
#include <sys/socket.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<semaphore.h>
#include <string.h>
#include <unistd.h>
//#include <netinet/net.h>
#include <fcntl.h>
/*
* Macro de verificare a erorilo
* Exemplu: 
* 		int fd = open (file_name , O_RDONLY);	
* 		DIE( fd == -1, "open failed");
*/

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)
	
/* Maximum size of data*/
#define BUFLEN 1500	

#endif
