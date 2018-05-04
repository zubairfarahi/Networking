#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
	int it;
	char buffer[255];
	struct hostent *messageDNS;
	struct in_addr address;
   if (argc < 3) {
       fprintf(stderr,"Usage : %s type name/ip\n", argv[0]);
       return -1;
   }
   if(strcmp(argv[1],"n") == 0) {
		messageDNS = gethostbyname(argv[2]);
		if(messageDNS == NULL) {
			herror("");
			return -1;
		}
		printf("nume = %s \n", messageDNS->h_name);
		printf("\naliasuri:\n");
		for(it = 0; messageDNS->h_aliases[it] != NULL; it++) {
			printf("%d: %s \n", it, messageDNS->h_aliases[it]);
		}
		printf("\ntype=%d\n", messageDNS->h_addrtype);
		printf("\nip-uri:\n");
		for(it = 0; messageDNS->h_addr_list[it] != NULL; it++) {
			memcpy(&address, messageDNS->h_addr_list[it], messageDNS->h_length);
			printf("%d: %s \n", it, inet_ntoa(address));
		}
	} else {
		if(strcmp(argv[1],"a") == 0) {
			inet_aton(argv[2], &address); 
			messageDNS = gethostbyaddr(&address, 4, AF_INET);
			if(messageDNS == NULL) {
				herror("");
				return -1;
			}
			printf("nume = %s \n", messageDNS->h_name);
			printf("\naliasuri:\n");
			for(it = 0; messageDNS->h_aliases[it] != NULL; it++) {
				printf("%d: %s \n", it, messageDNS->h_aliases[it]);
			}
			printf("\ntype=%d\n", messageDNS->h_addrtype);
			printf("\nip-uri:\n");
			for(it = 0; messageDNS->h_addr_list[it] != NULL; it++) {
				memcpy(&address, messageDNS->h_addr_list[it], messageDNS->h_length);
				printf("%d: %s \n", it, inet_ntoa(address));
			}
		} else {
			fprintf(stderr,"Wrong First Argument, Usage : %s (n 'or' a) name/ip\n", argv[0]);
			return -1;
		}
	}
	return 0;
}
