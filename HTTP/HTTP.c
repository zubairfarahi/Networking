#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define HTTP_PORT 80
#define MAXLEN 500

ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

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

void send_command(int sockfd, char sendbuf[], char *expected) {
  char recvbuf[MAXLEN];
  int nbytes=1;
  char CRLF[3];
  
  CRLF[0] = 13; CRLF[1] = 10; CRLF[2] = 0;
  strcat(sendbuf, CRLF);
  //printf("Trimit: %s", sendbuf);
  write(sockfd, sendbuf, strlen(sendbuf));
  int sem = 0;
  while(nbytes>0)
  {  
    nbytes = Readline(sockfd, recvbuf, MAXLEN - 1);  
    // recvbuf[nbytes] = 0;
        printf("%s", recvbuf);
     // if(strcmp(recvbuf,"\n") == 0 ) {
     //    break;
     // }

  }
}

int main(int argc, char **argv) {
  int sockfd;
  int port = 80;
  struct sockaddr_in servaddr;
  char server_ip[10];
  char sendbuf[MAXLEN]; 
  char recvbuf[MAXLEN];

  if (argc != 2) {
    printf("Utilizare: ./send_msg adresa_server");
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

  if (inet_aton(argv[1], &servaddr.sin_addr) <= 0 ) {
    printf("Adresa IP invalid.\n");
    exit(-1);
  }
    
  
  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    printf("Eroare \n");
    exit(-1);
  }

  sprintf(sendbuf, "GET /cn/wiki/lab/cn1/lab00 HTTP/1.1\nHost:elf.cs.pub.ro\n\n");
  send_command(sockfd, sendbuf, "HTTP");

  close(sockfd);
}