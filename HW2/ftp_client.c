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
    int num;
    FILE *f = fopen("banner2.txt", "r");
    while( (num = fgetc(f) ) != EOF) {
      //sleep(0.4);
      printf("%c", num);
    }

    printf("\n\nlink [ ip_addr]: Try to connect with host [ip_addr]\n");
    printf("send [filename]: Send file to host\n");
    printf("leave          : Exit Program\n\n");

    char newString[10][10], str1[100];
    int j = 0, ctr = 0, i;
    printf("What is your Cmd?\n");
    fgets(str1, sizeof(str1), stdin);
    for(i = 0; i <= (strlen(str1)); ++i) {
      if(str1[i] == ' ' || str1[i] == '\0') {
        newString[ctr][j] == '\0';
        ctr++;
        j = 0;
      } else {
        newString[ctr][j] = str1[i];
        j++;
      }
    }
    
    // for(i = 0; i < ctr; ++i)
    //   printf("%s\n", newString[i]);

    if(ctr != 1 && ctr != 2) {
      printf("Incorrect Input\n");
      return -1;
    }
    
    if ( !(strcmp(newString[0]), "leave")) {
      printf("Bye Bye\n");
      return 0;
    }
    if( (strcmp(newString[0], "link"))){
       printf("Incorrect Input\n");
      return -1;
    }

    //char *ip = "127.0.0.1";
    //char *ip = "192.168.32.137";
    char *ip = newString[1];
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

    struct sockaddr_in sockaddr;
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

    char newString2[100], str2[100], str3[100];
    int j2 = 0, ctr2 = 0, i2;
    printf("What's your Cmd?\n");
    // fgets(str2, sizeof(str2), stdin);
    // for(i2 = 0; i2 <= (strlen(str2)); ++i2) {
    //   if(str2[i2] == ' ' || str2[i2] == '\0') {
    //     newString2[ctr2][j2] == '\0';
    //     ctr2++;
    //     j2 = 0;
    //   } else {
    //     newString2[ctr2][j2] = str2[i2];
    //     j2++;
    //   }
    // }
    // // for(i2 = 0; i2 < ctr2; ++i2)
    // //   printf("%s\n", newString2[i2]);

    // if(ctr2 != 1 && ctr2 != 2) {
    //   printf("Incorrect Input\n");
    //   return -1;
    // }
    
    // if ( !(strcmp(newString2[0]), "leave")) {
    //   printf("Bye Bye\n");
    //   return 0;
    // }
    // if( (strcmp(newString2[0], "send"))){
    //    printf("Incorrect Input\n");
    //   return -1;
    // }
    scanf("%s %s", &str2, &str3);
    printf("%s %s\n", str2, str3);

    if ( (strcmp(str2), "leave")) {
      printf("Bye Bye\n");
      return 0;
    }
    if( (strcmp(str2, "send"))){
       printf("Incorrect Input\n");
      return -1;
    }


    FILE *fp, *huff_fp;
    char *filename = str3;
    printf("%s\n", filename);
    fp = fopen(filename, "r");
    if(fp == NULL) {
      perror("[-]File Reading Error\n");
      return -1;
    }
    huff_fp = fopen("compressed", "w");
    if(huff_fp == NULL) {
      perror("[-]File Reading Error\n");
      return -1;
    }

    compress(fp, huff_fp);
    fclose(huff_fp);

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

    huff_fp = fopen("compressed", "r");
    int num_bytes;
    char buffer[6291456];
    while(!feof(huff_fp)) {
      num_bytes = fread(buffer, sizeof(char), sizeof(buffer), huff_fp);
      printf("[+]fread %d bytes, ", num_bytes);
      num_bytes = write(socketfd, buffer, num_bytes);
      printf("Sending %d bytesn.\n", num_bytes);
    }
    /**
     * 1. fread() loads fp's file into buffer.
     * 2. with size/time: sizeof(char)/sizeof(buffer)
     * 3. print current bytes
     * 4. In write(), loads buffer into socketfd,
     * 5. and the third argu limits the size for each transmission, instead of sizeof(buffer).
     * https://wenchiching.wordpress.com/2009/10/14/linux-c-socketclientserver-transfer-file傳送檔案/
     */


    printf("\n[+]File Sent Successfully !!\n");

    close(socketfd);
    fclose(fp);
    fclose(huff_fp);
    printf("[+]Socket Closed.\n");

    return 0;
}