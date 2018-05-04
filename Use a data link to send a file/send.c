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

int main(int argc, char** argv){
	init (HOST, PORT);
	msg t;
	
    //  Send file name
	sprintf (t.payload, argv[1]);
	t.len = strlen (t.payload);
	send_message (&t);

    //  get notification that it received the package
	if (recv_message(&t)<0){
    	perror("receive error");
  	}
  	else {
    	printf("[%s] Got reply with payload: %s\n", argv[0], t.payload);

    	FILE *fs = fopen (argv[1], "r");

        //  Send file size
        fseek (fs, 0, SEEK_END);
        int fsize = ftell(fs);
        sprintf (t.payload, "%d", fsize);
        //  sending a package with the file size
        send_message (&t);

        fseek (fs, 0, SEEK_SET);

        //  get ack (package received)
        if (recv_message(&t) < 0) {
            perror ("no response after sending file size");
        } else {

            // Send file content
    	    while (fgets (t.payload, 1024, fs)) {
	    	    t.len = strlen(t.payload);

                //  send the message read
    	    	send_message (&t);

                printf("[%s] Got reply with payload: %s\n", argv[0], t.payload);

                if (recv_message(&t) < 0) {
                    break;
                }
    	    }
        }

        fclose (fs);
  	}

  	return 0;
}
