#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <sys/socket.h>
#endif

#include <cstdio>
#include <iostream>
#include <pthread.h>

using namespace std;

void task1() {
  for(int i = 0; i < 7; i++) {
    puts("Task 1");
  }
}

void task2() {
  for(int i = 0; i < 8; i++) {
    puts("task 2");
  }
}

int main(int argc, char *argv[]) {

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

  int fd;
  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("kentang cannot create socket");
    return 0;
  }
  printf("'asu'\n");
  printf("%d\n", fd);
  printf("%d\n", argc);
  for(int i = 0; i < argc; i++) {
    printf("%d : %s\n", i, argv[i]);
  }
  return 0;
}