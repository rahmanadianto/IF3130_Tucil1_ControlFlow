#include "udp_dcomm.h"

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

/* Functino declaration */
static Byte* receive_char(int sockfd, QTYPE* queue) {
/**
 * Insert code here.
 * Read a character from socket and put it to the receiver buffer
 * If the number of characters in the receive buffer is aboce certain
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

int main(int argc, char *argv[]) {
  Byte c;
  // insert code here to bind socket to the port number given in argv[1]
  // Initialize XON/XOFF flags
  // create child process
  
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