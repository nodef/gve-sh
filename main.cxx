#include <cstdio>
#include "inc/main.hxx"

using namespace std;




int main(int argc, char** argv) {
  UnixSocketServer server;
  server.bind("/tmp/socket-gve");
  server.listen([](int client_fd) {
    printf("client_fd=%d\n", client_fd);
  });
  printf("Hello World!\n");
  return 0;
}
