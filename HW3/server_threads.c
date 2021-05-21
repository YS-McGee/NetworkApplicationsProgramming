/**
 * <Better Comments>
 * * IMPORTANT
 * ! MMMM
 * ? _Atomic
 * ? static
 * ? pthread_detach()
 * TODO: unicast
 * TODO: offline msg
 * @param myParm this is parm
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CLIENTS 5	// less than this number
#define BUFFER_SZ 2048

static _Atomic unsigned int cli_count = 0;
static int uid = 1;

/* Client structure */
typedef struct{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	int is_online;
	char name[32];
	char offline_msg[BUFFER_SZ];
} client_t;

client_t *clients[MAX_CLIENTS];

/**
 * ! 靜態初始化
 * link: https://blog.xuite.net/mb1016.flying/linux/26293042
 */
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;			

/**
 * ! 7/7 
 * 
 */
void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

/**
 * ! 6/7 string trim: \0 string terminator
 * 
 */
void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

/**
 * ! 5/7 Print ip addr
 * 
 */
void print_client_addr(struct sockaddr_in addr){
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr & 0xff00) >> 8,
        (addr.sin_addr.s_addr & 0xff0000) >> 16,
        (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

/**
 * ! 4/7 Add clients to queue
 * 
 */
void queue_add(client_t *cl){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		// array 最小為空的 index
		if(!clients[i]){
			clients[i] = cl;
			clients[i]->is_online = 1;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/**
 * ! 3/7 Remove clients to queue
 * 
 */
void queue_remove(client_t *cl){
	pthread_mutex_lock(&clients_mutex);

	int uid = cl->uid;

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/**
 * ! 2/7 Send message to all clients except sender
 * 
 */
void send_message(char *s, int uid){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if (clients[i]->is_online == 0) {
					// save offline_msg to buffer
					//printf("check\n");
					//sprintf(clients[i]->offline_msg, "%s", s);
					strcat(clients[i]->offline_msg, s);
					continue;
				}
				if(write(clients[i]->sockfd, s, strlen(s)) < 0){
					printf("%s %d\n", clients[i]->name, clients[i]->uid);
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

void print_list() {
	// Avoid race condition at queue remove
	//sleep(1);

	printf("\n==========================\nOnline:\n--------------------------\n");
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(clients[i] != NULL && clients[i]->is_online == 1)
			printf("%s uid: %d\n", clients[i]->name, clients[i]->uid);
	}

	printf("\n==========================\nOffline:\n--------------------------\n");
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(clients[i] != NULL && clients[i]->is_online == 0)
			printf("%s uid: %d\n", clients[i]->name, clients[i]->uid);
	}
	printf("Online Client count: %d\n\n", cli_count);
}

void send_list(client_t *cl) {
	pthread_mutex_lock(&clients_mutex);

	char s[10];
	
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(clients[i] != NULL) {
			sprintf(s, "%s %d\n", clients[i]->name, clients[i]->is_online);
			if(write(cl->sockfd, s, strlen(s)) < 0) {
				perror("ERROR: write to descriptor failed");
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/**
 * ! 1/7 Handle all communication with the client
 * 
 */
void *handle_client(void *arg){

	char buff_out[BUFFER_SZ], off_msg[1];
	char name[32];
	int leave_flag = 0;
	int name_dup = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	// Name Validation
	int receive = recv(cli->sockfd, name, 32, 0);
	if( receive <= 0 || strlen(name) <  2 || strlen(name) >= 32-1){
		printf("Didn't enter the name.\n");
		leave_flag = 1;
	} else{
		// Check for duplicate name && is_from offline
		for(int i=0; i < MAX_CLIENTS; ++i) {
			if(clients[i] != NULL) {
				if(!strcmp(clients[i]->name, name)) {
					if(clients[i]->is_online == 1) {
						name_dup = 1;
						leave_flag = 1;
					} else {
						// back online
						sprintf(cli->offline_msg, "%s\n", clients[i]->offline_msg);
						
						clients[i]->is_online = 1;
						// printf("%s uid: %d, sockfd: %d\nmsg:%s\n", 
						// 	clients[i]->name, clients[i]->uid, clients[i]->sockfd, cli->offline_msg);
						//queue_remove(clients[i]);
					}
				}
			}
		}
		// dup name send
		sprintf(buff_out, "%d", name_dup);
		if(write(cli->sockfd, buff_out, strlen(buff_out)) < 0) {
			perror("ERROR: write to descriptor failed");
		}
		bzero(buff_out, BUFFER_SZ);
		if(name_dup != 1) {
			strcpy(cli->name, name);
			// offline_msg send
			if(strlen(cli->offline_msg) != 0) {
				if(write(cli->sockfd, cli->offline_msg, strlen(cli->offline_msg)) < 0) {
					perror("ERROR: write to descriptor failed");
				}
				bzero(cli->offline_msg, BUFFER_SZ);
			} else {
				sprintf(buff_out, "%s", "0-0");
				//bzero(buff_out, BUFFER_SZ);
				if(write(cli->sockfd, buff_out, strlen(buff_out)) < 0) {
					perror("ERROR: write to descriptor failed");
				}
			}
			bzero(buff_out, BUFFER_SZ);
			//sprintf(buff_out, "%s has joined (uid %d) (sockfd: %d)\n%s\n", cli->name, cli->uid, cli->sockfd, cli->offline_msg);
			sprintf(buff_out, "%s has joined (uid %d)\n", cli->name, cli->uid);
			printf("%s", buff_out);
			print_list();
			send_message(buff_out, cli->uid);
		}
	}

	bzero(buff_out, BUFFER_SZ);

	while(1){
		if (leave_flag) {
			break;
		}
		
		receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0){
			if(strstr(buff_out, "list")) {
				send_list(cli);
				str_trim_lf(buff_out, strlen(buff_out));
				printf("%s asked for %s\n", cli->name, buff_out);
			} else if (strstr(buff_out, "bye")) {
				cli->is_online = 0;
				sprintf(buff_out, "%s is offline (uid %d)\n", cli->name, cli->uid);
				printf("%s\n", buff_out);
				send_message(buff_out, cli->uid);
				cli_count--;

				print_list();
				while(1) {
					if(cli->is_online == 1) {
						// if(strlen(cli->offline_msg) != 0) {
						// 	printf("%s (sockfd: %d uid: %d)have offline msg\n%s\n", 
						// 		cli->name, cli->sockfd, cli->uid, cli->offline_msg);
						// 	bzero(cli->offline_msg, BUFFER_SZ);
						// }
						// printf("%s uid:%d terminated\n", cli->name, cli->uid);
						bzero(buff_out, BUFFER_SZ);
						close(cli->sockfd);
						queue_remove(cli);
						free(cli);
						pthread_detach(pthread_self());

						return NULL;
					} 
				}
			} else if(strlen(buff_out) > 0){
				send_message(buff_out, cli->uid);
				str_trim_lf(buff_out, strlen(buff_out));
				printf("%s: %s\n", cli->name, buff_out);
			}
		} else if (receive == 0 || strcmp(buff_out, "exit") == 0){
			sprintf(buff_out, "%s has left (uid %d)\n", cli->name, cli->uid);
			printf("%s\n", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		} else {
			printf("ERROR: -1\n");
			sprintf(buff_out, "%s has left (uid %d)\n", cli->name, cli->uid);
			printf("%s\n", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}
	bzero(cli->offline_msg, BUFFER_SZ);
	close(cli->sockfd);
	/* Delete client from queue and yield thread */
	if(leave_flag == 1) {
		queue_remove(cli);
  		free(cli);
  		cli_count--;
	}
	print_list();

	pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char **argv){

	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	int option = 1;
	int listenfd = 0, connfd = 0;
  	struct sockaddr_in serv_addr;
  	struct sockaddr_in cli_addr;
  	pthread_t tid;

  	/* Socket settings */
  	listenfd = socket(AF_INET, SOCK_STREAM, 0);
  	serv_addr.sin_family = AF_INET;
  	serv_addr.sin_addr.s_addr = inet_addr(ip);
  	serv_addr.sin_port = htons(port);

  	/* Ignore pipe signals */
	/**
	 * ! void (*signal(int sig, void (*func)(int)))(int)
	 * link: https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
	 * 
	 *   當服務器close一個連線時，若client端接著發資料。根據TCP協議的規定，會收到一個RST響應，
	 * client再往這個伺服器傳送資料時，系統會發出一個SIGPIPE訊號給程序，告訴程序這個連線已經斷開了，不要再寫了。
	 * 	　根據訊號的預設處理規則SIGPIPE訊號的預設執行動作是terminate(終止、退出),所以client會退出。
	 * 若不想客戶端退出可以把SIGPIPE設為SIG_IGN
	 * link:　https://codertw.com/%E7%A8%8B%E5%BC%8F%E8%AA%9E%E8%A8%80/430366/
	 */
	signal(SIGPIPE, SIG_IGN);

	/**
	 * ! int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	 * set to reuse port | addr
	 * option = 1 -> enabled
	 */
	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
    	return EXIT_FAILURE;
	}

	/* Bind */
  	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    	perror("ERROR: Socket binding failed");
    	return EXIT_FAILURE;
  	}

  	/* Listen */
  	if (listen(listenfd, 10) < 0) {
    	perror("ERROR: Socket listening failed");
    	return EXIT_FAILURE;
	}

	printf("=== WELCOME TO THE CHATROOM ===\n");

	while(1){
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;
		cli->is_online = 0;
		cli->offline_msg[0] = 0;

		/* Check if max clients is reached */
		if( ( cli_count + 1 ) == MAX_CLIENTS){
			char buff_out[] = "Max clients reached. Rejected: ";
			printf("%s", buff_out);
			send(connfd, buff_out, sizeof(buff_out), 0);
			print_client_addr(cli_addr);
			printf(":%d uid: %d\n", cli_addr.sin_port, cli->uid);
			
			//bzero(buff_out, BUFFER_SZ);
			// ==3747==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffdf196fa40 at pc 0x7f0cc5004f2d bp 0x7ffdf196f920 sp 0x7ffdf196f0c8
			close(connfd);
			continue;
		}

		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);

		/* Reduce CPU usage */
		sleep(1);
	}

	return EXIT_SUCCESS;
}