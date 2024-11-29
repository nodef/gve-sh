#include <cstdio>
#include <cstdint>
#include <string>
#include <string_view>
#include "inc/main.hxx"
#include "commands.hxx"

using namespace std;




#pragma region SUPPORT
template <class G>
#pragma endregion




#pragma region MAIN
/**
 * Read a string buffer, where each string is null-terminated.
 * @param buffer buffer to read
 * @param size size of the buffer
 * @returns strings in the buffer
 */
inline vector<const char*> readStringBuffer(const char *buffer, size_t size) {
  vector<const char*> argv;
  const char *begin = buffer;
  const char *end   = buffer + size;
  while (begin < end) {
    argv.push_back(begin);
    begin += strlen(buffer) + 1;
  }
  return argv;
}


inline void handleLoadCommand(UnixSocketServer &server, int client_fd, vector<DiGraph>& graphs, Options& o) {
  string_view inputFile = o["input-file"];
  int inputGraph = graphs.size();
  DiGraph x;
  printf("Loading graph G%d for client %d from %s ...\n", inputGraph, client_fd, inputFile.c_str());
  readMtxOmpW(x, file, weighted);
}


/**
 * Handle data from a client.
 * @param server server to handle
 * @param client_fd client file descriptor
 */
inline void handleRecieve(UnixSocketServer &server, int client_fd) {
  // Read the data from the client.
  char buffer[65536];
  size_t recieved = server.receive(client_fd, buffer, sizeof(buffer));
  printf("Client %d sent %zu bytes\n", client_fd, recieved);
  uint16_t *packetType = (uint16_t*) buffer;
  if (*packetType != 0x0000) {
    server.send(client_fd, "ERROR: Invalid packet format\n");
    printf("Client %d sent packet with invalid format.\n", client_fd);
    return;
  }
  // Parse the data.
  vector<const char*> argv = readStringBuffer(buffer, recieved);
  Options o = readOptions(argv.size(), argv.data());
  if (o["error"]!="") {
    server.send(client_fd, "ERROR: " + o["error"] + "\n");
    printf("Client %d sent invalid command.\n", client_fd);
  }
  if (o["help"]=="1") {
    server.send(client_fd, helpMessage());
    printf("Client %d requested help.\n", client_fd);
  }
}



/**
 * Main function.
 * @param argc number of arguments
 * @param argv array of arguments
 * @returns exit code
 */
int main(int argc, char **argv) {
  using K = uint32_t;
  using V = float;
  const char *socketFile = "/tmp/socket-gve";
  if (argc>1) socketFile = argv[1];
  UnixSocketServer server;
  // State information.
  vector<DiGraph<K, K, V>*> graphs;
  vector<vector<K>*> vectors;
  // Handle connection from a client.
  auto onConnect  = [&]() {
    int client_fd = server.accept();
    printf("Client %d connected.\n", client_fd);
  };
  // Handle disconnection from a client.
  auto onDisconnect = [&](int client_fd) {
    server.disconnect(client_fd);
    printf("Client %d disconnected.\n", client_fd);
  };
  auto onRecieve    = [&](int client_fd) { handleRecieve(server, client_fd); };
  // Start the server.
  server.bind(socketFile);
  server.listen(onRecieve, onConnect, onDisconnect);
  return 0;
}
#pragma endregion
