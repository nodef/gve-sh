#pragma once
#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>

using std::function;




#pragma region CLASSES
/**
 * Unix socket server.
 */
class UnixSocketServer {
  #pragma region TYPES
  public:
  /** Callback type for when a client disconnects (client_fd). */
  using on_disconnect_type = function<void(int)>;
  /** Callback type for when a client connects (). */
  using on_connect_type = function<void()>;
  /** Callback type for when data is received (client_fd). */
  using on_recieve_type = function<void(int)>;
  #pragma endregion


  #pragma region CONSTANTS
  protected:
  /** Maximum number of events to be returned by epoll_wait(). */
  static constexpr int MAX_EVENTS = 32;
  #pragma endregion


  #pragma region DATA
  protected:
  /** Socket file descriptor. */
  int socket_fd;
  /** Epoll file descriptor. */
  int epoll_fd;
  /** Server address. */
  sockaddr_un addr;
  /** Epoll events. */
  epoll_event events[MAX_EVENTS];
  #pragma endregion


  #pragma region METHODS
  public:
  /**
   * Close the server.
   */
  inline void close() {
    // Close the socket and epoll instance.
    if (socket_fd >= 0) {
      ::close(socket_fd);
      socket_fd = -1;
    }
    if (epoll_fd >= 0) {
      ::close(epoll_fd);
      epoll_fd = -1;
    }
    // Remove the socket file.
    ::unlink(addr.sun_path);
  }


  /**
   * Disconnect a client.
   * @param client_fd client socket file descriptor
   */
  inline void disconnect(int client_fd) {
    // Remove the client socket from the epoll instance.
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) < 0) throw "Failed to remove client socket from epoll!";
    // Close the client socket.
    ::close(client_fd);
  }


  /**
   * Receive data from a client.
   * @param client_fd client socket file descriptor
   * @param data data buffer
   * @param len length of data buffer
   * @returns number of bytes received
   * @throws char* if data could not be received
   */
  inline size_t receive(int client_fd, void *data, size_t len) {
    size_t num_bytes = ::read(client_fd, data, len);
    if (num_bytes < 0) throw "Failed to read from client!";
    return num_bytes;
  }


  /**
   * Send data to a client.
   * @param client_fd client socket file descriptor
   * @param data data to send
   * @param len length of data to send
   * @throws char* if data could not be sent
   */
  inline void send(int client_fd, const void *data, size_t len) {
    if (::write(client_fd, data, len) < 0) throw "Failed to write to client!";
  }


  /**
   * Send a string to a client.
   * @param client_fd client socket file descriptor
   * @param data string to send
   * @throws char* if data could not be sent
   */
  inline void send(int client_fd, const char *data) {
    send(client_fd, data, strlen(data));
  }


  /**
   * Bind the server to a socket file.
   * @param path path to the socket file
   * @throws char* if socket could not be bound
   */
  inline void bind(const char *path) {
    // Set up the server address.
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    // Remove any existing socket file.
    ::unlink(path);
    // Bind the socket to the server address.
    if (::bind(socket_fd, (sockaddr*) &addr, sizeof(addr)) < 0) throw "Failed to bind socket to address!";
  }


  /**
   * Accept an incoming connection.
   * @returns client socket file descriptor
   * @throws char* if connection could not be accepted
   */
  inline int accept() {
    sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = ::accept(socket_fd, (sockaddr*) &client_addr, &client_addr_len);
    if (client_fd < 0) throw "Failed to accept incoming connection!";
    // Add the client socket to the epoll instance, listening for incoming data and disconnects.
    epoll_event event;
    event.events  = EPOLLIN | EPOLLRDHUP;
    event.data.fd = client_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) throw "Failed to add client socket to epoll!";
    return client_fd;
  }


  /**
   * Start listening for incoming connections, data, and disconnects.
   * @param onRecieve callback for when data is received (client_fd)
   * @param onConnect callback for when a client connects ()
   * @param onDisconnect callback for when a client disconnects (client_fd)
   * @note this method will block indefinitely
   */
  inline void listen(on_recieve_type onRecieve, on_connect_type onConnect=nullptr, on_disconnect_type onDisconnect=nullptr) {
    // Set up in listening mode.
    if (::listen(socket_fd, SOMAXCONN) < 0) throw "Failed to listen for connections!";
    // Add the server socket to the epoll instance.
    epoll_event event;
    event.events  = EPOLLIN;
    event.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) throw "Failed to add server socket to epoll!";
    // Listen for events.
    while (true) {
      // Wait for events.
      int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
      if (num_events < 0) throw "Failed to wait for events!";
      // Handle events.
      for (int i=0; i<num_events; ++i) {
        // Accept incoming connection.
        if (events[i].data.fd == socket_fd) {
          if (onConnect) onConnect();
          else accept();
        }
        // Handle events from a client
        else {
          int client_fd = events[i].data.fd;
          // Client has sent data.
          if (events[i].events & EPOLLIN) {
            onRecieve(client_fd);
          }
          // Client has disconnected.
          else if (events[i].events & EPOLLRDHUP) {
            if (onDisconnect) onDisconnect(client_fd);
            else disconnect(client_fd);
          }
          // Client socket error.
          else if (events[i].events & EPOLLERR) {
            if (onDisconnect) onDisconnect(client_fd);
            else disconnect(client_fd);
            throw "Client socket error!";
          }
        }
      }
    }
  }
  #pragma endregion


  #pragma region CONSTRUCTORS
  public:
  /**
   * Create a new Unix socket server.
   * @throws char* if socket could not be created
   */
  UnixSocketServer()
  : socket_fd(-1), epoll_fd(-1) {
    // Create socket.
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) throw "Failed to create socket!";
    // Create an epoll instance.
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) throw "Failed to create epoll instance!";
  }

  /**
   * Destroy the Unix socket server.
   */
  ~UnixSocketServer() {
    close();
  }
  #pragma endregion
};




/**
 * Unix socket client.
 */
class UnixSocketClient {
  #pragma region TYPES
  public:
  /** Callback type for when data is received (). */
  using on_recieve_type = function<void()>;
  #pragma endregion


  #pragma region CONSTANTS
  protected:
  /** Maximum number of events to be returned by epoll_wait(). */
  static constexpr int MAX_EVENTS = 32;
  #pragma endregion


  #pragma region DATA
  protected:
  /** Socket file descriptor. */
  int socket_fd;
  /** Epoll file descriptor. */
  int epoll_fd;
  /** Client address. */
  sockaddr_un addr;
  /** Epoll events. */
  epoll_event events[MAX_EVENTS];
  #pragma endregion


  #pragma region METHODS
  public:
  /**
   * Close the client.
   */
  inline void close() {
    // Close the socket and epoll instance.
    if (socket_fd >= 0) {
      ::close(socket_fd);
      socket_fd = -1;
    }
    if (epoll_fd >= 0) {
      ::close(epoll_fd);
      epoll_fd = -1;
    }
  }


  /**
   * Receive data from the server.
   * @param data data buffer
   * @param len length of data buffer
   * @returns number of bytes received
   * @throws char* if data could not be received
   */
  inline size_t receive(void *data, size_t len) {
    size_t num_bytes = ::recv(socket_fd, data, len, 0);
    if (num_bytes < 0) throw "Failed to recieve from server!";
    return num_bytes;
  }


  /**
   * Send data to the server.
   * @param data data to send
   * @param len length of data to send
   * @throws char* if data could not be sent
   */
  inline void send(const void *data, size_t len) {
    if (::send(socket_fd, data, len, 0) < 0) throw "Failed to send to server!";
  }


  /**
   * Connect to the server.
   * @param path path to the socket file
   * @throws char* if connection could not be established
   */
  inline void connect(const char *path) {
    // Set up the server address.
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    // Connect to the server.
    if (::connect(socket_fd, (sockaddr*) &addr, sizeof(addr)) < 0) throw "Failed to connect to server!";
  }


  /**
   * Start listening for incoming data.
   * @param onRecieve callback for when data is received ().
   * @note this method will block indefinitely
   */
  inline void listen(on_recieve_type onRecieve) {
    // Add the client socket to the epoll instance.
    epoll_event event;
    event.events  = EPOLLIN;
    event.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) throw "Failed to add client socket to epoll!";
    // Listen for events.
    while (true) {
      // Wait for events.
      int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
      if (num_events < 0) throw "Failed to wait for events!";
      // Handle events.
      for (int i=0; i<num_events; ++i) {
        // Client has sent data.
        if (events[i].events & EPOLLIN) {
          onRecieve();
        }
        // Client socket error.
        else if (events[i].events & EPOLLERR) {
          close();
          throw "Client socket error!";
        }
      }
    }
  }
  #pragma endregion


  #pragma region CONSTRUCTORS
  public:
  /**
   * Create a new Unix socket client.
   * @throws char* if socket could not be created
   */
  UnixSocketClient()
  : socket_fd(-1), epoll_fd(-1) {
    // Create socket.
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) throw "Failed to create socket!";
    // Create an epoll instance.
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) throw "Failed to create epoll instance!";
  }


  /**
   * Destroy the Unix socket client.
   */
  ~UnixSocketClient() {
    close();
  }
  #pragma endregion
};
#pragma endregion
