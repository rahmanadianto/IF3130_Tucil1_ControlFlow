#ifdef _WIN32
  #include <Winsock2.h>
#else
  #include <sys/socket.h>
#endif

#include <cstdio>
#include <iostream>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

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

  int fd;
  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("kentang cannot create socket");
    return 0;
  }
  printf("%d\n", fd);
  printf("%d\n", argc);
  for(int i = 0; i < argc; i++) {
    printf("%d : %s\n", i, argv[i]);
  }

  thread t1 = thread(task1);
  thread t2 = thread(task2);

  t2.join();
  t1.join();
  return 0;
}