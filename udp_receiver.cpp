/**
 * Written by Luqman A. Siswanto
 * This code is easier to understand than you 
 * File : udp_receiver.h
 */

#include "udp_dcomm.h"

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <sys/socket.h>
#endif


using namespace std;

#define DELAY 500
#define SIZE 8

Byte buffer[SIZE];
QTYPE receiver_queue = {0, 0, 0, SIZE, buffer};
QTYPE *receiver_pointer = &receiver_queue;

Byte sent_xonxoff = XON;
bool sent_xon = false;
bool sent_xoff = false;

/* Socket */
int sockfd;

/* Function declaration */
static Byte* receive_char(int sockfd, QTYPE* queue) {
/**
 * Insert code here.
 * Read a character from socket and put it to the receiver buffer
 * If the number of characters in the receive buffer is above certain
 * leve, then send XOFF and set a flag (why?).
 * Return a pointer to the buffer where data is put.
 */
}

static Byte* q_get(QTYPE* queue, Byte* data) {
  Byte* current;
  // nothing in queue
  if(!queue->count) return NULL;

  /**
  * Insert code here.
  * Retrieve data from buffer, save it to "current" and "data"
  * If the number of characters in the receive buffer is below certain leve,
  * then send XON.
  * Increment front index and check for wraparound.
  */
}

/* paddr: print the IP address in a standard decimal dotted format */
void paddr(unsigned char *a) {
  printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

int main(int argc, char *argv[]) {
  Byte c;
  // insert code here to bind socket to the port number given in argv[1]
  // Initialize XON/XOFF flags
  // create child process
  
  // creating UDP socket
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("cannot create socket\n");
    return 0;
  }

  struct sockaddr_in myaddr;
  struct sockaddr_in remaddr;
  socklen_t addrlen = sizeof(remaddr);
  int recvlen;

  // initiating for binding
  int port = argc > 1? atoi(argv[1]) : DEFAULT_PORT;
  memset((char*) &myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, "127.0.0.1", &myaddr.sin_addr.s_addr);
  myaddr.sin_port = htons(port);


  // binding socket to valid IP Address and port
  if(bind(sockfd, (struct sockaddr*) &myaddr, sizeof(myaddr)) < 0) {
    perror("bind failed");
    return 0;
  }

  /*
  char* host = (char*) "google.com";
  struct hostent* hp;

  hp = gethostbyname(host);
  for(int i = 0; hp->h_addr_list[i] != 0; i++) {
    paddr((unsigned char*) hp->h_addr_list[i]);
  }
  */
  while(1) {
    printf("waiting on port %d\n", port);
    recvlen = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*) &remaddr, &addrlen);
    printf("received %d bytes\n", recvlen);
    if(recvlen > 0) {
      buffer[recvlen] = 0;
      printf("received this message : %s", buffer);
    }
  }

  /*** IF PARENT PROCESS ***/
  while(1) {
    c = *(receive_char(sockfd, receiver_pointer));
    if(c == Endfile) {
      exit(0);
    }
  }

  /*** ELSE IF CHILD PROCESS *****/
  while(1) {
    // call q_get
    // can introduce some delay here
  }
  return 0;
}