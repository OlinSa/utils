#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

using namespace std;

#define MAXLEN 1024
#define MAX_OPEN_FD 1024
#define SERV_PORT 8000

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, efd;
    char buf[MAXLEN];
    struct sockaddr_in cliaddr, servaddr;
    socklen_t clilen = sizeof(cliaddr);
    struct epoll_event ep[MAX_OPEN_FD], tep;

    if (-1 == (listenfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        cout << __LINE__ << strerror(errno) << endl;
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (-1 == (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))))
    {
        cout << __LINE__ << strerror(errno) << endl;
        return -1;
    }
    listen(listenfd,20);

    efd = epoll_create(MAX_OPEN_FD);
    tep.events = EPOLLIN;
    tep.data.fd = listenfd;

    if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep))
    {
        cout << __LINE__ << strerror(errno) << endl;
        return -1;
    }
    for (;;)
    {
        size_t nready = epoll_wait(efd, ep, MAX_OPEN_FD - 1, -1);
        for (unsigned int i = 0; i < nready; i++)
        {
            //new client connect
            if (ep[i].data.fd == listenfd)
            {
                connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep))
                {
                    cout << __LINE__ << strerror(errno) << endl;
                    return -1;
                }
            }
            else
            { //read client dataa
                connfd = ep[i].data.fd;
                int bytes = read(connfd, buf, MAXLEN);
                //peer socket is close
                if (bytes == 0)
                {
                    epoll_ctl(efd, EPOLL_CTL_DEL, connfd, NULL);
                    close(connfd);
                    cout << "client[" << i << "] is close" << endl;
                }
                else
                {
                    for (int j = 0; j < bytes; ++j)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    // write to client
                    write(connfd, buf, bytes);
                }
            }
        }
    }
    return 0;
}