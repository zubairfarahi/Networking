
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv)
{
	
	init(HOST,PORT);
	msg sendedMessage, recivedMessage;
	char buffer[1396];
	int *headerData;
	char *rawData;
	int it;

	strcpy(buffer, "Ciocirlan este smecher mafiot");

	sendedMessage.len = strlen(buffer) + 1 + sizeof(int);
	headerData = (int *) sendedMessage.payload;
	*headerData = 0;
	headerData++;
	rawData = (char *) headerData;
	strcpy(rawData, buffer);
	*(rawData + strlen(buffer)) = '\0';

	/*calculez cheksum
	*/
	headerData--;
	*headerData = checksum(rawData);
	
	for( it = 0; it < noIteration; it++ ) {
		
		send_message(&sendedMessage);

		
		if (recv_message(&recivedMessage) < 0) {
			perror("receive error");
		}
		else {
			printf("[%s] Got reply with payload: %s\n"
				,argv[0],recivedMessage.payload);
		}

        }

	return 0;
}
