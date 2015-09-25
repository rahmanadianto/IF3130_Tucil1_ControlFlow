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
#define ESC 27       /* ESC key */

/* XON/XOFF protocol */
#define XON (0x11)
#define XOFF (0x13)

/* Constant */
#define BYTESIZE 256  /* The maximum value of a byte */
#define MAXLEN 1024   /* Maximum messages length */

#define DEFAULT_PORT 21234

#include <bits/stdc++.h>

typedef unsigned char Byte;

/**
 * bila fron == rear maka queue kosong
 * rear adalah index yg selalu kosong
 */
struct QTYPE {
  QTYPE(unsigned int c, unsigned int f, unsigned int r, unsigned int m, Byte* d) {
    count = c;
    fron = f;
    rear = r;
    maxsize = m;
    data = d;
  }
  void push(Byte c) {
    data[rear] = c;
    rear = (rear + 1) % maxsize;
    count++;
    assert(count < maxsize);
  }
  void pop() {
    fron = (fron + 1) % maxsize;
    count--;
    assert(count >= 0);
  }
  int size() {
    return count;
  }
  bool empty() {
    return count == 0;
  }
  Byte front() {
    return data[fron];
  }
  unsigned int count;
  unsigned int fron;
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