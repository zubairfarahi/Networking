#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
    msg r,t;
    init(HOST,PORT);

    //  Receive filename
    if (recv_message(&r)<0){
        perror("Receive message");
        return -1;
    }

    printf("[%s] Got msg with payload: %s\n",argv[0],r.payload);

    //  Check if the file exists
    int fs = open (r.payload, O_RDONLY);

    if (fs < 0) {
        printf("Could not open %s\n", r.payload);
        return -1;
    }
    close (fs);

    sprintf(t.payload,"ACK(%s)",r.payload);
    t.len = r.len;

    //  Send notification (as received package)
    send_message(&t);

    //   get the file size
    if (recv_message(&r) < 0){
        perror("Receive message");
        return -1;
    }

    int fileSize = atoi (r.payload);

    printf ("%d\n", fileSize);

    sprintf(t.payload,"ACK(%s)",r.payload);

    //  Send ack (package with size received)
    send_message (&r);
 
    FILE *fd = fopen ("file.out", "a");

    int countBytes = 0;

    while (recv_message(&r)) {
        sprintf(t.payload,"ACK(%s)",r.payload);
        printf("[%s] Got msg with payload: %s\n",argv[0],r.payload);

        countBytes += r.len;
        fputs (r.payload, fd);
        sprintf(t.payload,"ACK(%s)",r.payload);

        //  Request message packages
        send_message (&t);

        if (countBytes >= fileSize) {
            break;
        }
    }

    fclose (fd);

    return 0;
}
