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

    char *ip = "127.0.0.1";
    char *filename = "THY_ART_IS_MURDER.mp3";

    FILE *fp;

    struct sockaddr_in sockaddr;

    // Create Socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) {
      perror("[-]Socket Error\n");
      return -1;
    }
    printf("[+]Socket Created !!\n");
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

    sockaddr.sin_family = AF_INET;                 // Address family IPv4
    sockaddr.sin_port = htons(9090);               // host to network short: Big endian
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    
    int conn = connect(socketfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    if(conn < 0) {
      perror("[-]Socket error\n");
      return -1;
    }
    printf("[+]Connected to Server!\n");
    /**
     * ! int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
     * * https://man7.org/linux/man-pages/man2/connect.2.html
     *
     * The connect() system call connects the socket referred to by the
       file descriptor sockfd to the address specified by addr. The
       addrlen argument specifies the size of addr.
     *
     * @param addr:
     * struct sockaddr_in {
     *   __kernel_sa_family_t	sin_family;	  // Address Family
     *   __be16		sin_port;	                // Port number
     *   struct in_addr	sin_addr;	          // Internet address
     * };
     * * sockaddr vs. sockaddr_in: https://stackoverflow.com/questions/21099041/why-do-we-cast-sockaddr-in-to-sockaddr-when-calling-bind
     * 
     */

    fp = fopen(filename, "rb");             // "rb" for non-textual file
    if(fp == NULL) {
      perror("[-]File Reading Error\n");
      return -1;
    }

    // char c[1024];         // Buffer
    // recv(fd, &c, sizeof(c), 0);
    // printf("%s\n", c);
    /**
     * ! ssize_t recv(int sockfd, void *buf, size_t len, int flags);
     * The recv(), recvfrom(), and recvmsg() calls are used to receive
       messages from a socket. 
     */

    // Send file
    // char data[1024] = {0};
    // while (fgets(data, 1024, fp) != NULL) {
    //   if (send(socketfd, data, sizeof(data), 0) == -1) {
    //     perror("[-]Sending data Error\n");
    //     return -1;
    //   }
    //   bzero(data, 1024);
    // }

    int num_bytes;
    char buffer[6291456];
    while(!feof(fp)) {
      num_bytes = fread(buffer, sizeof(char), sizeof(buffer), fp);
      printf("[+]fread %d bytes, ", num_bytes);
      num_bytes = write(socketfd, buffer, num_bytes);
      printf("Sending %d bytesn.\n", num_bytes);
    }
    printf("\n[+]File Sent Successfully !!\n");

    close(socketfd);
    printf("[+]Socket Closed.\n");

    return 0;
}