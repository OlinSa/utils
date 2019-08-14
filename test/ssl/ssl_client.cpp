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
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace std;

#define MAXBUF 1024

typedef struct {
    int sockfd;
    SSL_CTX *sslCtx;
} NETWORK_ENV_T;

void ShowCerts(SSL *ssl) {
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("无证书信息！\n");
}

int SockInit(const char *host, int port) {
    int sockfd = -1;
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_aton(host, (struct in_addr *) &addr.sin_addr.s_addr) == 0) {
        perror("host");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        perror("connect");
        return -1;
    }

    return sockfd;

}

SSL_CTX *SSLInit(NETWORK_ENV_T *env) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *sslCtx = SSL_CTX_new(SSLv23_client_method());
    if (!sslCtx) {
        ERR_print_errors_fp(stdout);

    }
    return sslCtx;
}


NETWORK_ENV_T *NetInit(const char *host, int port) {

    NETWORK_ENV_T *env = new NETWORK_ENV_T();
    if (env) {
        if (nullptr == (env->sslCtx = SSLInit(env))) {
            cout << "ssl init failed" << endl;
            delete env;
            env = NULL;
            goto do_exit;
        }
        if (-1 == (env->sockfd = SockInit(host, port))) {
            perror("sockInit failed");
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
        SSL_CTX_free(env->sslCtx);
        close(env->sockfd);
        delete env;
    }
}


int main(int argc, char *argv[]) {
    int ret = -1;
    int len;
    const char *host = "127.0.0.1";
    int port = 6666;
    SSL *ssl = nullptr;

    char buf[MAXBUF + 1] = {0};
    NETWORK_ENV_T *env = NetInit(host, port);
    if (!env) {
        cout << "net init failed" << endl;
        return -1;
    }
    ssl = SSL_new(env->sslCtx);
    SSL_set_fd(ssl, env->sockfd);

    if (-1 == SSL_connect(ssl)) {
        ERR_print_errors_fp(stderr);
        goto do_exit;
    }
    std::cout << "SSL connection using: " << SSL_get_cipher(ssl) << std::endl;
    ShowCerts(ssl);

    memset(buf, 0, sizeof(buf));

    len = SSL_write(ssl, "hello", strlen("hello"));
    if (len < 0) {
        cout << "write msg failed!" << ERR_reason_error_string(SSL_get_error(ssl, len)) << endl;
        goto do_exit;
    }

    len = SSL_read(ssl, buf, MAXBUF);
    if (len < 0) {
        cout << "read msg failed!" << strerror(errno) << endl;
        goto do_exit;
    }
    cout << "recv msg:" << buf << endl;


    SSL_shutdown(ssl);
    SSL_free(ssl);
    ret = 0;

    do_exit:

    NetExit(env);
    return ret;
}


