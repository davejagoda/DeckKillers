#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
  Useful resources:
  https://beej.us/guide/bgnet/html/split/index.html
  https://stackoverflow.com/questions/4046616/sockets-how-to-find-out-what-port-and-address-im-assigned
  https://stackoverflow.com/questions/10758632/how-to-create-a-udp-server-in-c

  Depending on the ulimit of the user running this program, one
  session may not be enough to exhaust all the ports. To exhaust them
  all, you can suspend the program and start another. Repeat this
  until the program can't open any more sockets. Note that once you've
  exhausted all the ports, the computer will not work work well
  (e.g. if you exhaust all UDP ports, you won't be able to make a new
  DNS request). This is a Deck Killer.
*/

int help()
{
  printf("options\n");
  printf("-h: show this output\n");
  printf("-t: exhaust TCP ports\n");
  printf("-u: exhaust UDP ports\n");
  return 0;
}

int tcp()
{
  printf("tcp\n");
  int count = 0;
  int status = 0;
  struct addrinfo hints;
  struct addrinfo *res;  // will point to the results
  struct addrinfo *p;    // to walk the results

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

  if ((status = getaddrinfo(NULL, "0", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  while (status == 0) {
    for (p = res; p != NULL; p = p->ai_next) {
      int sockfd = 0;
      void *addr;
      char *ipver;

      sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      status = bind(sockfd, res->ai_addr, res->ai_addrlen);
      status = listen(sockfd, 0);
      if (status != 0) {
        perror("listen");
      }

      struct sockaddr_in sin;
      socklen_t len = sizeof(sin);
      if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
      else
        count ++;
        if (p->ai_family == AF_INET) { // IPv4
          ipver = "IPv4";
        } else { // IPv6
          ipver = "IPv6";
        }
      printf("%s port number %d\n", ipver, ntohs(sin.sin_port));
    }
  }
  printf("Opened %d sockets\n", count);
  printf("Press return to continue\n");
  getchar();
  freeaddrinfo(res); // free the linked-list
}

int udp()
{
  printf("udp\n");
  int count = 0;
  int status = 0;
  struct addrinfo hints;
  struct addrinfo *res;  // will point to the results
  struct addrinfo *p;    // to walk the results

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM;  // UDP datagram sockets
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

  if ((status = getaddrinfo(NULL, "0", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  while (status == 0) {
    for (p = res; p != NULL; p = p->ai_next) {
      int sockfd = 0;
      void *addr;
      char *ipver;

      sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      status = bind(sockfd, res->ai_addr, res->ai_addrlen);

      struct sockaddr_in sin;
      socklen_t len = sizeof(sin);
      if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
      else
        count ++;
        if (p->ai_family == AF_INET) { // IPv4
          ipver = "IPv4";
        } else { // IPv6
          ipver = "IPv6";
        }
      printf("%s port number %d\n", ipver, ntohs(sin.sin_port));
    }
  }
  printf("Opened %d sockets\n", count);
  printf("Press return to continue\n");
  getchar();
  freeaddrinfo(res); // free the linked-list
}

int main (int argc, char **argv)
{
  int c;

  while ((c = getopt (argc, argv, "htu")) != -1)
    switch (c)
      {
      case 'h':
        help();
        break;
      case 't':
        tcp();
        break;
      case 'u':
        udp();
        break;
      }
  return 0;
}
