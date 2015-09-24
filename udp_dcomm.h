/*
 * File : udp_dcomm.h
 */

#ifndef _DCOMM_H_
#define _DCOMM_H_

/** ASCII CONST */
#define SOH 1        /* Start of Header Character */
#define STX 2        /* Start of Text Character */
#define ETX 3        /* End of Text Character */
#define ENQ 5        /* Enquiry Character */
#define ACK 6        /* Acknowledgement */
#define BEL 7        /* Message Error Warning */
#define CR 13        /* Carriage Return */
#define LF 10        /* Line Feed */
#define NAK 21       /* Negativ Acknowledgement */
#define Endfile 26   /* End of file Character */
#define ESC 27       /*ESC key  */

/* XON/XOFF protocol */
#define XON (0x11)
#define XOFF (0x13)

/* Constant */
#define BYTESIZE 256  /* The maximum value of a byte */
#define MAXLEN 1024   /* Maximum messages length */

typedef unsigned char Byte;

struct QTYPE {
  unsigned int count;
  unsigned int front;
  unsigned int rear;
  unsigned int maxsize;
  Byte* data;
};

struct MESGB {
  unsigned int soh;
  unsigned int stx;
  unsigned int etx;
  Byte checksum;
  Byte msgno;
  Byte* data;
};

#endif