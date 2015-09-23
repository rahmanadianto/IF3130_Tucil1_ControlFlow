#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <sys/socket.h>
#endif

#include <cstdio>
#include <iostream>

int main() {

  int fd;
  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("kentang cannot create socket");
    return 0;
  }


  return 0;
}