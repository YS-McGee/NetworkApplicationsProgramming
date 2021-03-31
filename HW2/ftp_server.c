#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

/**
 * * Important
 * ! E
 * ? tbc
 * TODO: add "recommanded way to view these files in vs code, use Better Comments"
 * @param myParam This param does bla bla bla
 */

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    /**
    * ! int socket(int domain, int type, int protocol); 
    * * https://man7.org/linux/man-pages/man2/socket.2.html
    *
    * Creates an endpoint for communication and returns a file
    * descriptor that refers to that endpoint. The file descriptor
    * returned by a successful call will be the lowest-numbered file
    * descriptor not currently open for the process.
    *
    * @param domain         The domain argument specifies the protocol\
    *                       family which will be used for communication.
    * @param AF_INET        IPv4 Internet protocols
    *          
    * @param type @param SOCK_STREAM
        Provides sequenced, reliable, two-way, connection-based
        byte streams.  An out-of-band data transmission mechanism
        may be supported.
    *
    * Normally only a single @param protocol exists to support a
    * particular socket type within a given protocol family, in which
    * case protocol can be specified as 0.
    */

    // printf("%d\n", fd); // print the file descriptor

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;              // Address family IPv4
    sockaddr.sin_port = htons(9090);            // host to network short: Big endian
    sockaddr.sin_addr.s_addr = INADDR_ANY;      // * INADDR_ANY means socket will bound to all local interface
    
    int conn = bind(fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    /**
     * ! int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
     * When a socket is created with socket(2), it exists in a name
       space (address family) but has no address assigned to it.  bind()
       assigns the address specified by addr to the socket referred to
       by the file descriptor sockfd.  addrlen specifies the size, in
       bytes, of the address structure pointed to by addr.
       Traditionally, this operation is called “assigning a name to a
       socket”.
     * 
     */ 
    return 0;
}