#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

/**
 * * Important
 * ! E
 * ? tbc
 * TODO: Huffman
 * TODO: UI: function select / Text Banner (figlet) / Coloured Text / Help /  
 * TODO: Listen ... repeat dot dot dot / dot dot dot
 * @param myParam This param does bla bla bla
 */

void compress(FILE *fp, FILE *huff_fp) {

    int k, h = 0, i, a = 0, base = 1;
    int c;
    int b[8], bit;
    char hex[2];
    
    while( (c = fgetc(fp) ) != EOF) {
        // for(k = 1 << 7; k > 0; k = k / 2, ++h) {
        //     (c & k) ? (b[h] = 1) : (b[h] = 0);
        // }
        // h = 0;
        sprintf(hex, "%2x", c);
        for(i = 1; i >= 0; --i) {
            if(hex[i] >= '0' && hex[i] <= '9') {
                a += (hex[i] - 48) * base;
                base *= 16;
            }
            else if(hex[i] >= 'A' && hex[i] <= 'F') {
                a += (hex[i] - 55 ) * base;
                base *= 16;
            }
            else if(hex[i] >= 'a' && hex[i] <= 'f') {
                a += (hex[i] -87) * base;
                base *= 16;
            }
        }
        fprintf(huff_fp, "%c", a);
        base = 1;
        a = 0;
        //printf("%c ", c);
        //puts(hex);
        //fwrite(hex, sizeof(char), 2, huff_fp);
    }
    //printf("\n");
}

void decompress(FILE *fp, FILE *new_fp) {

    int count, a = 0, i, base = 1, num;
    char c[1];

    // while(1) {
    //     if( ( count = fread(&c, sizeof(char), 2, fp) ) == 0) {
    //         printf("\n");
    //         return;
    //     }
    //     for(i = 1; i >= 0; --i) {
    //         if(c[i] >= '0' && c[i] <= '9') {
    //             a += (c[i] - 48) * base;
    //             base *= 16;
    //         }
    //         else if(c[i] >= 'A' && c[i] <= 'F') {
    //             a += (c[i] - 55 ) * base;
    //             base *= 16;
    //         }
    //         else if(c[i] >= 'a' && c[i] <= 'f') {
    //             a += (c[i] -87) * base;
    //             base *= 16;
    //         }
    //     }
    //     fprintf(new_fp, "%c", a);
    //     base = 1;
    //     a = 0;
    // }
    while( (num = fgetc(fp) ) != EOF) {
        fprintf(new_fp, "%c", num);
    }
}

int main() {

    // sudo apt install figlet toilet
    // print ASCII text banner on terminal
    int num;
    FILE *f = fopen("banner.txt", "r");
    while( (num = fgetc(f) ) != EOF) {
      sleep(0.4);
      printf("%c", num);
    }

    // Create Socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) {
      perror("[-]Socket Error\n");
      return -1;
    }
    printf("[+]Server Socket Created !!\n");
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
    
    char *ip = "127.0.0.1";
    //char *ip = "192.168.32.137";

    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

    // Specify an address for the socket
    server_addr.sin_family = AF_INET;              // Address family IPv4
    server_addr.sin_port = htons(9090);            // host to network short: Big endian
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int b = bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(b < 0) {
      perror("[-]Binding Error\n");
      return -1;
    }
    printf("[+]Binding Successful\n");
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
    
    int u;
    b = listen(socketfd, 5);
    if(b == 0) {
      printf("[+]Listening....\n");
    } else {
      perror("[-]Listening Error\n");
      return -1;
    }
    /**
      * !   int listen(int sockfd, int backlog);
      * listen() marks the socket referred to by sockfd as a passive
        socket, that is, as a socket that will be used to accept incoming
        connection requests using accept(2).
      * On success, zero is returned.  On error, -1 is returned, and
        errno is set to indicate the error.
     */

    addr_size = sizeof(new_addr);
    int new_sock = accept(socketfd, (struct sockaddr*)&new_addr, &addr_size);      // the socket used to send data
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
    // char d[256] = "These messages are from the server.";
    // send(c, d, sizeof(d), 0);
    /**
     * ! ssize_t send(int sockfd, const void *buf, size_t len, int flags);
     * The send() call may be used only when the socket is in a
       connected state (so that the intended recipient is known).  The
       only difference between send() and write(2) is the presence of
       flags.  With a zero flags argument, send() is equivalent to
       write(2).
     */ 

    FILE *huff_fp, *new_fp;

    huff_fp = fopen("compressed", "w");
    if (huff_fp == NULL) {
      perror("[-]File Creation Error(huff_fp)\n");
      return -1;
    }

    // Seek to the beginning of the file
    // fseek(fp, 0, SEEK_SET);

    // int n;
    // char buffer[6291456];
    // while(1) {
    //   n = recv(new_sock, buffer, 50, 0);
    //   if (n <= 0)
    //     break;
    //   fprintf(fp, "%s", buffer);
    //   bzero(buffer, 50);
    // }

    int num_byte;
    char buffer[6291456];
    while(1) {
      num_byte = read(new_sock, buffer, sizeof(buffer));
      if(num_byte == 0)
        break;
      printf("[+]read %d bytes, ", num_byte);
      num_byte = fwrite(buffer, sizeof(char), num_byte, huff_fp);
      printf("[+]fwrite %d bytes.\n", num_byte);
    }
    /**
     * 1. Uses read() to load the contents in new_sock into buffer, with each time maximum sizeof(buffer)Bytes
     * 2. if no bytes reveived, end loop.
     * 3. print current bytes.
     * 4. use fwrite() to load buffer into fp, each time sizeof(char)bytes, of num_bytes times.
     * 5. If too many times, useless data would be written, that why we use the return value of read().
     * 6. print current fwrite().
     */
    fclose(huff_fp);
    huff_fp = fopen("compressed", "r");
    if (huff_fp == NULL) {
      perror("[-]File Creation Error(huff_fp)\n");
      return -1;
    }
    new_fp = fopen("decomp", "w");
    if (new_fp == NULL) {
      perror("[-]File Creation Error\n");
      return -1;
    }
    decompress(huff_fp, new_fp);

    printf("\n[+]Data Written Successful !!\n");
    
    fclose(huff_fp);
    fclose(new_fp);
    close(new_sock);

    return 0;
}