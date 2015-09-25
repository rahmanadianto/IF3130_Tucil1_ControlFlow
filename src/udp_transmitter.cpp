/*
 * File : udp_transmitter.h
 * Usage: transmitter [receiver IP] [receiver port] [message.txt]
 */

#include "udp_dcomm.h"
#include <sys/socket.h>
#include <unistd.h> // close socket
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // inet_pton
#include <pthread.h>

#include <fstream>


// DELAY untuk ngeprint MENUNGGU XON dalam microsecond
#define DELAY 200000

/* file descriptor for connected socket */
int socketfd;
int socketclosed = 0;

/* address structure */
struct sockaddr_in transmitterAddr; 
struct sockaddr_in receiverAddr;

/* signal receiver XON or XOFF */
Byte sent_xonxoff = XON; // default set to XON

/*
	Connect to the service running on host:port.
	This function return 0 on failure, non-zero on success. 
*/
int binding(char *host, int port) {

	printf("Membuat socket untuk koneksi ke %s:%d...\n", host, port);

	/* Construct the receiver sockaddr_ structure */
  memset(&transmitterAddr, 0, sizeof(transmitterAddr));
  transmitterAddr.sin_family = AF_INET; // AF_INET it family code for IP4
  transmitterAddr.sin_addr.s_addr = htonl(INADDR_ANY); // let OS choose interface for this socket by give INADDR_ANY 
  transmitterAddr.sin_port = htons(0); // let the OS pick any available port number by specifying port 0

  /* Create the socket */
	if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	  perror("Failed to create socket");
	  return 0;
	}

	if(bind(socketfd, (struct sockaddr *) &transmitterAddr, sizeof(transmitterAddr)) < 0) {
	  perror("Failed to bind socket");
	  return 0;
	}

	/* Construct the transmitter sockaddr_ structure */
  memset(&receiverAddr, 0, sizeof(receiverAddr));
  receiverAddr.sin_family = AF_INET; // AF_INET it family code for IP4
  inet_pton(AF_INET, host, &receiverAddr.sin_addr.s_addr);
  receiverAddr.sin_port = htons(port);

 	return 1;
}

void* receiveSignalXONXOFF(void* threadid) {
	Byte buf;
	socklen_t addrlen = sizeof(receiverAddr);
	while (!socketclosed) {
		recvfrom(socketfd, &buf, 1, 0, (struct sockaddr *)&receiverAddr, &addrlen);
		if (buf == XOFF)
			sent_xonxoff = XOFF;
		else
			sent_xonxoff = XON;
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	if (argc > 3) {
		int bind_success = binding(argv[1], atoi(argv[2]));	
		if (!bind_success)
			return 0;

		char *filename = argv[3];

		/* Create thread for XON XOFF signal from receiver */
		pthread_t thread;
		pthread_create(&thread, NULL, receiveSignalXONXOFF, NULL);

		/* File message */
		std::ifstream infile;
		infile.open(filename);

		char c;
		int i = 0;
		socklen_t addrlen = sizeof(receiverAddr);
		while (infile.get(c)) {
			if(sent_xonxoff == XON) {
				if (!sendto(socketfd, &c, sizeof(char), 0, (struct sockaddr *)&receiverAddr, addrlen)) {
					perror("sendto failed");
				} else {
					printf("Mengirim byte ke-%d: %c\n", i + 1, c);
					i++;
				}
			}

			if (sent_xonxoff == XOFF) {
				printf("XOFF diterima.\n");
				/* waiting XON */
				int counter = 0;
				while(sent_xonxoff == XOFF) {
					if(counter == 0)
						printf("Menunggu XON...\n");
					counter = 1;
				}
			} 
		}

		close(socketfd); // destroy socket
		socketclosed = 1;
	}
	else {
		printf("Usage : transmitter [receiver IP] [receiver port] [message.txt]\n");
	}

	pthread_exit(NULL);
}
