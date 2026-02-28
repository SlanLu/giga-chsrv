#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

// ============ GLOBALS ============
const char *server_socket_name = "12345";
int server_socket_fd = -1;
constexpr int MAX_BACKLOG = 2;

int main(int argc, char *argv[]) {

  int connection_status = 0;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((connection_status =
           getaddrinfo(nullptr, server_socket_name, &hints, &servinfo)) != 0) {
    perror("get address error: ");
    exit(1);
  }

  printf("LOG: Successfully obtained address info - server address is: %s",
         servinfo->ai_addr->sa_data);

  server_socket_fd =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  if (server_socket_fd == -1) {
    perror("Socket connection error: ");
    exit(1);
  }

  printf("LOG: Successfully connected to socket %s using fd: %d\n",
         server_socket_name, server_socket_fd);

  int bind_result = 0;
  if ((bind_result = bind(server_socket_fd, servinfo->ai_addr,
                          servinfo->ai_addrlen)) == -1) {
    perror("Binding socket failed: ");
    exit(1);
  }

  printf("LOG: Successfully binded to the socket!\n");

  int opt = 1;
  if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
                 sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEADDR failed\n");
    exit(1);
  }

  freeaddrinfo(servinfo);

  if (listen(server_socket_fd, MAX_BACKLOG)) {
    perror("Listen function failed: ");
    exit(1);
  }

  return 0;
}
