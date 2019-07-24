#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "shm_com.h"

int main(int argc, char **argv)
{
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    char buffer[TEXT_SZ];
    int shmid;

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "Shmget failed!\n");
        exit(-1);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1)
    {
        fprintf(stderr, "Shmat failed!\n");
        exit(-1);
    }

    printf("Memory attached at %p\n", shared_memory);

    shared_stuff = (struct shared_use_st *)shared_memory;
    while (running)
    {
        while (shared_stuff->written_by_you == 1)
        {
            sleep(1);
            printf("Waiting for client...\n");
        }
        printf("Enter some text: ");
        fgets(buffer, TEXT_SZ, stdin);

        strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
        shared_stuff->written_by_you = 1;

        if (strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
    }

    if (shmdt(shared_memory) == -1)
    {
        fprintf(stderr, "Shmdt failed\n");
        exit(-1);
    }

    exit(0);
}