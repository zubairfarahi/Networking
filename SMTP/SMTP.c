#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define SMTP_PORT 25
#define MAXLEN 500

/**
 Read maximum maxlen bytes from the sockfd socket. Turn
  number of bytes read.
 */
ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = (char *)vptr;

    for ( n = 1; n < maxlen; n++ ) {	
	if ( (rc = read(sockd, &c, 1)) == 1 ) {
	    *buffer++ = c;
	    if ( c == '\n' )
		break;
	}
	else if ( rc == 0 ) {
	    if ( n == 1 )
		return 0;
	    else
		break;
	}
	else {
	    if ( errno == EINTR )
		continue;
	    return -1;
	}
    }

    *buffer = 0;
    return n;
}

/**
 Sends an SMTP command and awaits server response.
  The command must be in the sendbuf buffer.
  The expected stream contains the beginning of the answer that you have
  must send the server in case of success (e.g.
  250). If the answer signals an error, it exits the program.
 */
void send_command(int sockfd, char sendbuf[], char *expected) {
  char recvbuf[MAXLEN];
  int nbytes;
  char CRLF[3];
  
  CRLF[0] = 13; CRLF[1] = 10; CRLF[2] = 0;
  strcat(sendbuf, CRLF);
  printf("Send: %s", sendbuf);
  write(sockfd, sendbuf, strlen(sendbuf));
  nbytes = Readline(sockfd, recvbuf, MAXLEN - 1);
  recvbuf[nbytes] = 0;
  printf("I received: %s", recvbuf);
  if (strstr(recvbuf, expected) != recvbuf) {
    printf("We have received a hangout message from the server!\n");
    exit(-1);
  }
}

int main(int argc, char **argv) {
  int sockfd;
  int port = SMTP_PORT;
  struct sockaddr_in servaddr;
  char server_ip[10];
  char sendbuf[MAXLEN]; 
  char recvbuf[MAXLEN];

  if (argc != 2) {
    printf("Utilize: ./send_msg adresa_server");
    exit(-1);
  }
  strcpy(server_ip, argv[1]);

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	printf("Eroare \n");
	exit(-1);
  }  

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_aton(server_ip, &servaddr.sin_addr) <= 0 ) {
    printf("Adresa IP invalid.\n");
    exit(-1);
  }
    
 
  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    printf("Eroare \n");
    exit(-1);
  }

  Readline(sockfd, recvbuf, MAXLEN -1);
  printf("i received: %s\n", recvbuf);

  sprintf(sendbuf, "HELO 172.16.7.187");
  send_command(sockfd, sendbuf, "250");

  //...
  sprintf(sendbuf, "MAIL FROM:zubair.farahi@gmail.com");
  send_command(sockfd, sendbuf, "250");

  sprintf(sendbuf, "RCPT TO:extraaa2@gmail.com");
  send_command(sockfd, sendbuf, "250");

  sprintf(sendbuf, "DATA");
  send_command(sockfd, sendbuf, "354");

  sprintf(sendbuf, "Author:Farahi Zubair\nGroup:325CD\n\nWorking\n.");
  send_command(sockfd, sendbuf, "250");

  sprintf(sendbuf, "QUIT");
  send_command(sockfd, sendbuf, "221");
  close(sockfd);
}
  
