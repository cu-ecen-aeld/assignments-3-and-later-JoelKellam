#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <syslog.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>

#define LOG_FILE "/var/tmp/aesdsocketdata"
#define PORT 9000

struct sockaddr_in server_sockaddr, client_sockaddr;
int socket_desc, acc;
FILE *fp = NULL;
char replyMessage;

void sighandler();
void print_results();

int main(int argc, char *argv[]) {
  pid_t pid;
  char ipinput[INET_ADDRSTRLEN];
  int option_value = 1;

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = htons(PORT);
  server_sockaddr.sin_addr.s_addr = INADDR_ANY;

  if(argc==2 && argv[1][1] == 'd') 
  {
    pid = fork();
    if (setsid() < 0)
      exit(-1);
    if (pid > 0)
      exit(0);
  }

  if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) || bind(socket_desc, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1)
  {
    syslog(LOG_ERR, "SERVER failed to connect correctly.");
    exit(-1);
  }

  signal(SIGINT, sighandler);
  signal(SIGTERM, sighandler);
  fp = fopen(LOG_FILE, "a+");

  while(1) {
    if (listen(socket_desc, 5) < 0) 
    {
      exit(-1);
    }

    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
    acc = accept(socket_desc, (struct sockaddr *)&client_sockaddr, &peer_addr_size);
    if (acc == -1)
    {
      exit(-1);
    }

    inet_ntop(AF_INET, &(client_sockaddr.sin_addr), ipinput, INET_ADDRSTRLEN);
    print_results();

    syslog(LOG_USER, "Closed connection from %s", ipinput);
    signal(SIGTERM, sighandler);
  }
}

void print_results() {
  int result;
  char c;
  do 
  {
    result = recv(acc, &replyMessage, 1, MSG_WAITALL);
    if(result < 0) 
    {
      perror("recv");
      exit(-1);
    }
    fprintf(fp, "%c", replyMessage);
  } while (replyMessage != '\n');

    rewind(fp);
    while (!feof(fp)) {
      c = fgetc(fp);
      if(feof(fp))
        break;
      send(acc, &c, 1, 0);
    }
}

void sighandler(int signum) {
  printf("Caught signal %d, coming out...\n", signum);
  shutdown(acc, SHUT_RDWR);
  shutdown(socket_desc, SHUT_RDWR);
  close(acc);
  close(socket_desc);
  fclose(fp);
  remove(LOG_FILE);
  exit(0);
}