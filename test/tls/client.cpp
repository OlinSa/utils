#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#define CA_CERT "ca/ca-cert.pem"
#define CLIENT_CERT "server/server-cert.pem"

#define EXIT_IF_TRUE(x)                                    \
    do {                                                        \
        if(x) {                                                 \
            std::cout<<__FUNCTION__<<":"<< __LINE__<<std::endl; \
            exit(2);                                            \
        }                                                       \
    }while(0)

using namespace std;


int main(int argc, char *argvp[])
{
    SSL_METHOD *meth;
    SSL_CTX *ctx;
    SSL *ssl;

       //init SSL
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_BIO_strings();

    //使用SSL V3, V2
    EXIT_IF_TRUE((ctx=SSL_CTX_new(SSLv23_method())) == NULL);

    //要求对方校验证书
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

    //加载CA证书
    EXIT_IF_TRUE(!SSL_CTX_load_verify_locations(ctx, CA_CERT, NULL));

    //加载自己的证书
    EXIT_IF_TRUE (SSL_CTX_use_certificate_file(ctx, CLIENT_CERT, SSL_FILETYPE_PEM) <= 0) ; 


    int nFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(8812);
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(nFd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        perror("Connect ");
        exit(errno);
    }

    EXIT_IF_TRUE((ssl = SSL_new(ctx)) == NULL);
    SSL_set_fd(ssl, nFd);
    int n1 = SSL_connect(ssl);
    if (n1 == -1) {
        int n2 = SSL_get_error(ssl, n1);
 
        const char* p1 = SSL_state_string(ssl);
        cout<<p1<<endl;
        return -1;
    }
 
    char szBuffer[1024];
    // 进行操作
    sprintf(szBuffer, "this is from client %d", getpid());
    int nWriten = SSL_write(ssl, szBuffer, strlen(szBuffer));
 
    // 释放资源
    memset(szBuffer, 0, sizeof(szBuffer));
    int nLen = SSL_read(ssl, szBuffer, sizeof(szBuffer));
    cout<<"Get Len"<<nLen<<endl;
    cout<<szBuffer;
 
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(nFd);
 
    return 0;
}