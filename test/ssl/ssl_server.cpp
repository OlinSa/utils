#include <iostream>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace std;

typedef struct {
    int sockfd;
    SSL_CTX *sslCtx;
} NETWORK_ENV_T;


#define MAXBUF 1024

int SockInit(int port, int listenNum) {
    int sockfd = -1;
    struct sockaddr_in myAddr;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = PF_INET;
    myAddr.sin_port = htons(port);
    myAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &myAddr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        return -1;
    }
    if (listen(sockfd, listenNum) == -1) {
        perror("listen");
        return -1;
    }
    return sockfd;

}

SSL_CTX *SSLInit(NETWORK_ENV_T *env, const char *certificate, const char *privateKey) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *sslCtx = SSL_CTX_new(SSLv23_server_method());
    if (sslCtx) {

        if (0 >= (SSL_CTX_use_certificate_file(sslCtx, certificate, SSL_FILETYPE_PEM))) {
            ERR_print_errors_fp(stdout);
            SSL_CTX_free(sslCtx);
            sslCtx = NULL;
            goto do_exit;
        }
        if (0 >= (SSL_CTX_use_PrivateKey_file(sslCtx, privateKey, SSL_FILETYPE_PEM))) {
            ERR_print_errors_fp(stdout);
            SSL_CTX_free(sslCtx);
            sslCtx = NULL;
            goto do_exit;
        }
        if (!SSL_CTX_check_private_key(sslCtx)) {
            ERR_print_errors_fp(stdout);
            SSL_CTX_free(sslCtx);
            sslCtx = NULL;
            goto do_exit;
        }
    } else {
        ERR_print_errors_fp(stdout);
    }


do_exit:

    return sslCtx;
}


NETWORK_ENV_T *NetInit(int port, int listenNum, const char *certificateFile, const char *privateFile) {

    NETWORK_ENV_T *env = new NETWORK_ENV_T();
    if (env) {
        if (-1 == (env->sockfd = SockInit(port, listenNum))) {
            perror("sockInit failed");
            delete env;
            env = NULL;
            goto do_exit;
        }
        if (NULL == (env->sslCtx = SSLInit(env, certificateFile, privateFile))) {
            cout << "ssl init failed" << endl;
            delete env;
            env = NULL;
            goto do_exit;
        }
    }
    do_exit:
    return env;
}


void NetExit(NETWORK_ENV_T *env) {
    if (env) {
        close(env->sockfd);
        SSL_CTX_free(env->sslCtx);
        delete env;
    }
}


int main(int argc, char *argv[]) {
    int port = 6666;
    int listenNum = 10;

    SSL *ssl;

    char buf[MAXBUF + 1] = {0};

    X509*    client_certificate;
    const char *certificate = "CAcert.pem";
    const char *keytificate = "privkey.pem";
    NETWORK_ENV_T *env = NetInit(port, listenNum, certificate, keytificate);
    if (!env) {
        cout << "net init failed" << endl;
        return -1;
    }

    int newFd = -1;
    struct sockaddr_in peerAddr;
    socklen_t len = sizeof(struct sockaddr);

    while ((newFd = accept(env->sockfd, (struct sockaddr *) &peerAddr, &len)) != -1) {
        cout << "peer ip:" << inet_ntoa(peerAddr.sin_addr) << " port:" << ntohs(peerAddr.sin_port) << " fd:" << newFd
             << endl;

        ssl = SSL_new(env->sslCtx);
        SSL_set_fd(ssl, newFd);
        if (SSL_accept(ssl) == -1) {
            perror("accpet");
            close(newFd);
            continue;
        }

        std::cout<<"SSL connection using: "<<SSL_get_cipher(ssl)<<std::endl;
        client_certificate = SSL_get_peer_certificate (ssl);
        if (client_certificate != NULL) {
            std::cout << "Client certificate:" << std::endl;
        }

        memset(buf, 0, sizeof(buf));

        len = SSL_read(ssl, buf, MAXBUF);
        if (len > 0) {
            cout << "recv msg:" << buf << " len:" << len << endl;
        } else {
            cout << "recv failed:" << strerror(errno) << endl;
        }
        len = SSL_write(ssl, buf, strlen(buf));
        if (len > 0) {
            cout << "send msg:" << buf << endl;
        } else {
            cout << "send failed：" << strerror(errno) << endl;
        }
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(newFd);
    }


    NetExit(env);
}