#ifndef LIB
#define LIB

#define TYPE1		1
#define TYPE2		2
#define TYPE3		3
#define TYPE4		4
#define ACK_T1		"ACK(TYPE1)"
#define ACK_T2		"ACK(TYPE2)"
#define ACK_T3		"ACK(TYPE3)"

#define MSGSIZE		1400
#define PKTSIZE		1396

#define noIteration 20
unsigned char mask_byte(unsigned int x) {
	if( x > 8) {
		return 0;
	}
	unsigned int rez = 1;
	unsigned int it;
	for(it = 0; it < x; it++) {
		rez = (rez << 1);
	}
	return rez;
}

unsigned char xor_byte (unsigned char data) {
	unsigned char rez;
	rez = data & 1;
	int it;
	for(it = 1; it < 8; it++ ) {
		rez = rez ^ ((data / mask_byte(it) & 1));
	}
	return rez;
}
unsigned int checksum(char *data) {
	unsigned char Rez = 0;
	char *it = data;
	unsigned int Rez_final = 0;
	while(*it != '\0') {
		Rez = Rez ^ (*it);
		it++;
	}
	Rez_final = (unsigned int) xor_byte(Rez);
	return Rez_final;
}

typedef struct {
  	int len;
  	char payload[MSGSIZE];
} msg;

typedef struct {
	int checksum;
	char payload[PKTSIZE];	
} my_pkt;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);

#endif

