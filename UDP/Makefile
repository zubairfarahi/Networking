

CFLAGS = -Wall -g

# The port you listen to the server
PORT = 12345

# The IP address of the server
IP_SERVER = 172.27.102.214

# The file from where the client reads
FILE_CLIENT = client_random

# The file where the server is saved
FILE_SERVER = server_random

all: server client 

# Compileaza server.c
server: server.c

# Compileaza client.c
client: client.c

.PHONY: clean random_file run_server run_client

# Build a 10MB file (1024B * 10 ^ 5) == 10MB
random_file:
	dd if=/dev/urandom of=${FILE_CLIENT} bs=1024 count=10000


run_server:
	./server ${PORT} ${FILE_SERVER}
	md5sum ${FILE_SERVER}

run_client:
	./client ${IP_SERVER} ${PORT} ${FILE_CLIENT}

clean:
	rm -f server client ${FILE_CLIENT} ${FILE_SERVER}
