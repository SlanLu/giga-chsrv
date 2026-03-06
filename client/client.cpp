#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// ============ GLOBALS ============
const char *client_socket_name = "12345";
int client_socket_fd = -1;

int main(int argc, char *argv[]) {

  int connection_status = 0;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((connection_status = getaddrinfo("localhost", client_socket_name, &hints,
                                       &servinfo)) != 0) {
    perror("get address error: ");
    exit(1);
  }

  printf("LOG: Successfully obtained address info - client address is: %s",
         servinfo->ai_addr->sa_data);

  client_socket_fd =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  if (client_socket_fd == -1) {
    perror("Socket connection error: ");
    exit(1);
  }

  printf("LOG: Successfully connected to socket %s using fd: %d\n",
         client_socket_name, client_socket_fd);

  if (connect(client_socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
    perror("Connect function failed!\n");
    exit(1);
  }

  freeaddrinfo(servinfo);

  ssize_t bytes_rw = 0;
  char msg_buf[512] = "Hello!";

  bytes_rw = send(client_socket_fd, msg_buf, strlen(msg_buf), 0);
  if (bytes_rw < 0) {
    printf("LOG: error sending message to the server!\n");
  }
  recv(client_socket_fd, msg_buf, sizeof(msg_buf), 0);
  if (bytes_rw < 0) {
    printf("LOG: error occured while receiving ack from server!\n");
  } else {
    if (strncasecmp(msg_buf, "ack", 3) == 0) {
      printf("Server Successfully received our message!\n");
    } else {
      printf("Server couldn't receive our message properly!\n");
    }
  }

  close(client_socket_fd);

  return 0;
}
