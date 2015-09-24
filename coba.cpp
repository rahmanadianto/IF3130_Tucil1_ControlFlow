#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <sys/socket.h>
#endif

#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

  int fd;
  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("kentang cannot create socket");
    return 0;
  }
  printf("%d\n", fd);
  cout << argc << endl;
  for(int i = 0; i < 100; i++) {
    printf("%d : %s\n", i, argv[i]);
  }
  return 0;
}