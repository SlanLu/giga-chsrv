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

  freeaddrinfo(servinfo);

  return 0;
}
