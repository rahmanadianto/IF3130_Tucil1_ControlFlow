/**
 * Written by Luqman A. Siswanto
 * Understanding this code is easier than understanding you.
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

// delay in microseconds
#define DELAY 500000
#define SIZE 8
#define UPPER_LIMIT 4
#define LOWER_LIMIT 1

Byte buffer[SIZE];
Byte bufget[SIZE];
QTYPE receiver_queue = QTYPE(0, 0, 0, SIZE, buffer);
QTYPE *receiver_pointer = &receiver_queue;

Byte sent_xonxoff[0];
bool sent_xon;

/* Socket */
int sockfd;

/* Binding socket */
struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t addrlen;
int recvlen;
int cnt_receiver, cnt_consumer;
int port;

/* paddr: print the IP address in a standard decimal dotted format */
void paddr(unsigned char *a) {
  printf("%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
}

struct arg_struct {
  // nothing, just formality :)
};

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
  
  printf("Menunggu seseorang di ");
  paddr((unsigned char*) &myaddr.sin_addr.s_addr);
  printf(":%d ...\n", port);
  
  recvlen = recvfrom(sockfd, buffer, 1, 0, (struct sockaddr*) &remaddr, &addrlen);
  printf("Menerima byte ke-%d\n", ++cnt_receiver);
  queue->push(buffer[0]);
  if(queue->count > UPPER_LIMIT) {
    sent_xon = false;
    sent_xonxoff[0] = XOFF;
    sendto(sockfd, sent_xonxoff, 1, 0, (struct sockaddr*) &remaddr, (socklen_t) addrlen);
    printf("Buffer > minimum upperlimit. Mengirim XOFF\n");
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
  printf("Mengkonsumsi byte ke-%d: '%c'\n", ++cnt_consumer, data[0]);
  if(queue->count < LOWER_LIMIT && sent_xon == false) {
    sent_xon = true;
    sent_xonxoff[0] = XON;
    sendto(sockfd, sent_xonxoff, 1, 0, (struct sockaddr*) &remaddr, (socklen_t) addrlen);
    printf("Buffer < maximum lowerlimit. Mengirim XON.\n");
  }
  return data;
}

void *run_receive_char(void* arguments) {
  // IF PARENT PROCESS
  while(1) {
    Byte c = *(receive_char(sockfd, receiver_pointer));

    if(c == Endfile) {
      exit(0);
    }
  }
  pthread_exit(NULL);
}

void *run_q_get(void* arguments) {
  // ELSE IF CHILD PROCESS
  while(1) {
    q_get(receiver_pointer, bufget);
    usleep(DELAY);
    // call q_get
    // can introduce some delay here
  }
  pthread_exit(NULL);
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
  port = argc > 1? atoi(argv[1]) : DEFAULT_PORT;
  memset((char*) &myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, "127.0.0.1", &myaddr.sin_addr.s_addr);
  myaddr.sin_port = htons(port);

  printf("Binding pada alamat ");
  paddr((unsigned char*) &myaddr.sin_addr.s_addr);
  printf(":%d ...\n", port);


  // binding socket to valid IP Address and port
  if(bind(sockfd, (struct sockaddr*) &myaddr, sizeof(myaddr)) < 0) {
    perror("Binding gagal.");
    return 0;
  }
  printf("Binding berhasil\n\n");

/*
  char* host = (char*) "google.com";
  struct hostent* hp;

  hp = gethostbyname(host);
  for(int i = 0; hp->h_addr_list[i] != 0; i++) {
    paddr((unsigned char*) hp->h_addr_list[i]);
  }
  
  while(1) {
    printf("waiting on port %d\n", port);
    recvlen = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*) &remaddr, &addrlen);
    printf("received %d bytes\n", recvlen);
    if(recvlen > 0) {
      buffer[recvlen] = 0;
      printf("received this message : %s", buffer);
    }
  }
*/
  pthread_t receiver_thread;
  pthread_t consumer_thread;
  struct arg_struct args;
  sent_xon = true;
  sent_xonxoff[0] = XON;

  int ret = pthread_create(&receiver_thread, NULL, &run_receive_char, (void*) &args);
  if(ret != 0) {
    puts("Threading receiver gagal");
    exit(0);
  }

  int rec = pthread_create(&consumer_thread, NULL, &run_q_get, (void*) &args);
  if(rec != 0) {
    puts("Threading consumber gagal");
    exit(0);
  }

  pthread_exit(NULL);
  close(sockfd);
  return 0;
}