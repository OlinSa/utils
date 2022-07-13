#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
struct mmsgbuf {
  long type;
  char text[100];
};

int main(void) {
  int id = msgget(1234, 0);
  struct mmsgbuf sndbuf;
  struct mmsgbuf rcvbuf;

  pid_t pid = getpid();

  while (1) {
    memset(&sndbuf, 0x00, sizeof(sndbuf));
    memset(&rcvbuf, 0x00, sizeof(rcvbuf));

    sndbuf.type = pid;

    fgets(sndbuf.text, 100, stdin);

    msgsnd(id, &sndbuf, strlen(sndbuf.text), 0);
    printf("pid=%d", pid);
    for (int i = 0; i < strlen(sndbuf.text); i++) {
      printf("sndbuf[%d]=%x\n", i, sndbuf.text[i]);
    }
    msgrcv(id, &rcvbuf, 100, pid, 0);
    printf("--> %s\n", rcvbuf.text);
    sleep(1);
  }
}