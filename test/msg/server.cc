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

int id;
void handler(int s) {
  msgctl(id, IPC_RMID, 0);
  exit(0);
}
int main(void) {
  struct sigaction act;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGINT, &act, NULL);

  //创建消息队列
  id = msgget(1234, IPC_CREAT | 0644);
  if (id == -1) {
    perror("creat error!\n"), exit(1);
  }

  struct mmsgbuf rcvbuf;
  while (1) {
    memset(&rcvbuf, 0x00, sizeof(rcvbuf));
    int size = msgrcv(id, &rcvbuf, 100, 0, 0);
    printf("server:recv %d byte type=%ld text=%s\n", size, rcvbuf.type,
           rcvbuf.text);
    // rcvbuf.type = *(pid_t*)(rcvbuf.text);
    msgsnd(id, &rcvbuf, strlen(rcvbuf.text + sizeof(pid_t)) + sizeof(pid_t), 0);
  }
}