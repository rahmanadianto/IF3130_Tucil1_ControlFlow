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

		char *msg = "Hallo";
		socklen_t addrlen = sizeof(receiverAddr);
		for (int i = 0; i < strlen(msg); i++) {
			if (!sendto(socketfd, &msg[i], sizeof(char), 0, (struct sockaddr *)&receiverAddr, addrlen))
				perror("sendto failed");
			printf("Mengirim byte ke-%d: %c\n", i + 1, msg[i]);

			if (sent_xonxoff == XOFF) {
				printf("XOFF diterima.\n");
				/* waiting XON */
				while(sent_xonxoff != XON) {
					printf("Menunggu XON...\n");
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
