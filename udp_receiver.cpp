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
#define UPPER_LIMIT 4
#define LOWER_LIMIT 1

Byte buffer[SIZE];
QTYPE receiver_queue = QTYPE(0, 0, 0, SIZE, buffer);
QTYPE *receiver_pointer = &receiver_queue;

Byte sent_xonxoff = XON;
bool sent_xon = true;

/* Socket */
int sockfd;

/* Binding socket */
struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t addrlen;
int recvlen;

/* Function declaration */
static Byte* receive_char(int sockfd, QTYPE* queue) {
  /**
   * Insert code here.
   * Read a character from socket and put it to the receiver buffer
   * If the number of characters in the receive buffer is above certain
   * level, then send XOFF and set a flag (why?).
   * Return a pointer to the buffer where data is put.
   */

  // if XOFF then don't receive from socket
  if(sent_xon == false) return NULL;
  
  recvlen = recvfrom(sockfd, buffer, 1, 0, (struct sockaddr*) &remaddr, &addrlen);
  queue->push(buffer[0]);
  if(queue->count > UPPER_LIMIT) {
    sent_xon = false;
    sent_xonxoff = XOFF;
    sendto(sockfd, sent_xonxoff, strlen(sent_xonxoff), 0, (struct sockaddr*) &remaddr, addrlen);
  }
  return buffer;
}

static Byte* q_get(QTYPE* queue, Byte* data) {
  // nothing in queue
  if(queue->empty()) return NULL;

  /**
  * Insert code here.
  * Retrieve data from buffer, save it to "current" and "data"
  * If the number of characters in the receive buffer is below certain level,
  * then send XON.
  * Increment front index and check for wraparound.
  */

  data[0] = queue->front();
  queue->pop();
  if(queue->count < LOWER_LIMIT && sent_xon == false) {
    sent_xon = true;
    sent_xonxoff = XON;
    sendto(sockfd, sent_xonxoff, strlen(sent_xonxoff), 0, (struct sockaddr*) &remaddr, addrlen);
  }
  return data;
}

/* paddr: print the IP address in a standard decimal dotted format */
void paddr(unsigned char *a) {
  printf("%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
}

int main(int argc, char *argv[]) {
  Byte c;
  // insert code here to bind socket to the port number given in argv[1]
  // Initialize XON/XOFF flags
  // create child process
  
  // creating UDP socket
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Tidak dapat membuat socket\n");
    return 0;
  }
  printf("Socket created with descriptor %d\n", sockfd);
  

  // initiating for binding
  addrlen = sizeof(remaddr);
  int port = argc > 1? atoi(argv[1]) : DEFAULT_PORT;
  memset((char*) &myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, "127.0.0.1", &myaddr.sin_addr.s_addr);
  myaddr.sin_port = htons(port);

  printf("Binding pada ");
  paddr((unsigned char*) &myaddr.sin_addr.s_addr);
  printf(":%d ...\n", port);


  // binding socket to valid IP Address and port
  if(bind(sockfd, (struct sockaddr*) &myaddr, sizeof(myaddr)) < 0) {
    perror("Binding gagal.");
    return 0;
  }
  printf("Binding berhasil\n");

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
  close(fd);
  return 0;
}