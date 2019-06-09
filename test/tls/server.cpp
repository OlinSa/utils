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
#define SERVER_CERT "server/server-cert.pem"
#define SERVER_SECRET "server/server-key.pem"

void print_errors(void);

#define EXIT_IF_TRUE(x)                                    \
    do {                                                        \
        if(x) {                                                 \
            std::cout<<__FUNCTION__<<":"<< __LINE__<<std::endl; \
            print_errors();                                     \
            exit(2);                                            \
        }                                                       \
    }while(0)

using namespace std;

void print_errors(void)
{
    int flags, line;
    const char *data, *file;
    unsigned long code;

    code = ERR_get_error_line_data(&file, &line, &data, &flags);
    while(code)
    {
        cout<<"error code:0x"<<hex<<code<<" in "<< file<<" line:"<<dec<<line<<endl;
        if(data && (flags & ERR_TXT_STRING))
            cout<<"error data:"<< data<<endl;
        code = ERR_get_error_line_data(&file, &line, &data, &flags);
    }
}

int main(int argc, char *argv[])
{
    SSL_CTX     *ctx;
    SSL         *ssl;
    X509        *client_cert;

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
    EXIT_IF_TRUE (SSL_CTX_use_certificate_file(ctx, SERVER_CERT, SSL_FILETYPE_PEM) <= 0) ; 

    //加载私钥
    EXIT_IF_TRUE (SSL_CTX_use_PrivateKey_file(ctx, SERVER_SECRET, SSL_FILETYPE_PEM) <= 0) ;  

    //判定私钥准确性
    EXIT_IF_TRUE(!SSL_CTX_check_private_key(ctx)); 

    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8812);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int opt = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

    if(-1 == bind(listenFd, (struct sockaddr*)&addr, sizeof(struct sockaddr))) {
        cout<<"bind failed"<<endl;
        return -1;
    }

    listen(listenFd, 10);

    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));
    socklen_t len = sizeof(cli_addr);
    int acceptFd = accept(listenFd, (struct sockaddr*)&cli_addr, &len);
    if(acceptFd == -1) {
        cout<<"accept failed"<<endl;
        return -1;
    }
    cout<<"Accept a connect from "<<inet_ntoa(cli_addr.sin_addr)<<":"<<ntohs(cli_addr.sin_port)<<endl;

    //SSL connect
    EXIT_IF_TRUE((ssl = SSL_new(ctx)) == NULL);
    SSL_set_fd(ssl, acceptFd);
    if(-1 == SSL_accept(ssl)) {
        cout<<SSL_state_string_long(ssl)<<endl;
        print_errors();
        return -1;
    }
    char szBuf[1024] = {0};
    
    int nLen = SSL_read(ssl, szBuf, sizeof(szBuf));
    cout<<"Get Len:"<<nLen<<endl;
    cout<<"szBuf:"<<szBuf<<endl;

    strcat(szBuf, " this is from server");
    SSL_write(ssl, szBuf, strlen(szBuf));
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(acceptFd);
    close(listenFd);
    
    return 0;
}