/**
 * dcomm.h
 */

#ifndef _DCOMM_H_
#define _DCOMM_H_

/** ASCII CONST */
#define SOH 1
#define STX 2
#define ETX 3
#define ENQ 5
#define ACK 6
#define BEL 7
#define CR 13
#define LF 10
#define NAK 21
#define Endfile 26
#define ESC 27

/* XON/XOFF protocol */
#define XON (0x11)
#define XOFF (0x13)

#define BYTESIZE 256
#define MAXLEN 1024

typedef unsigned char Byte;

struct QTYPE {
  unsigned int count;
  unsigned int front;
  unsigned int rear;
  unsigned int maxsize;
  Byte* data;
};

struct MESGB {

};


#endif