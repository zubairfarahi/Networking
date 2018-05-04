#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001


int main(int argc,char** argv){
	
	init(HOST,PORT);
	
	msg sendedMessage, recivedMessage;

	char buffer[1392];
	int *headerData;
	char *rawData;
	int messageLen;
	int it;
	int checksum_recived;
	int checksum_calculated;

	for(it = 0; it < noIteration; it++ ) {	
		
		if (recv_message(&recivedMessage) < 0){
			perror("Receive message");
			return -1;
		}
		printf("[%s] Got msg with payload: %s\n",
			argv[0],recivedMessage.payload);

			
		messageLen = recivedMessage.len;
		
		if(messageLen < 4 ) {
			perror("Receive message");
			return -1;
		}

		
		headerData = (int *) recivedMessage.payload;
		checksum_recived = *headerData;
		headerData++;
		rawData = (char *) headerData;

		
		strcpy(buffer, rawData);

		
		checksum_calculated = checksum(buffer);

		if(checksum_calculated != checksum_recived) {
			printf("[%s] \n"
				,"Checksum Error");
		}
		
		sprintf(sendedMessage.payload,"ACK(%s)",
			recivedMessage.payload);
		sendedMessage.len = strlen(sendedMessage.payload+1);
		send_message(&sendedMessage);

	}

	return 0;
}
