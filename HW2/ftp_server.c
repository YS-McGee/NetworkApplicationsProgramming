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

    int a = socket(AF_INET, SOCK_STREAM, 0);
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
    
    int b = bind(a, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
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

    int c = listen(a, 5);
    /**
      * !   int listen(int sockfd, int backlog);
      * listen() marks the socket referred to by sockfd as a passive
        socket, that is, as a socket that will be used to accept incoming
        connection requests using accept(2).
      * On success, zero is returned.  On error, -1 is returned, and
        errno is set to indicate the error.
     */
    int d = accept(a, 0, 0);      // the socket used to send data
    /**
     * ! int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
     *  The accept() system call is used with connection-based socket
       types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first
       connection request on the queue of pending connections for the
       listening socket, sockfd, creates a new connected socket, and
       returns a new file descriptor referring to that socket.  The
       newly created socket is not in the listening state.  The original
       socket sockfd is unaffected by this call.
     
     */

    return 0;
}