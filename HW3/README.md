# Chatroom C Program

## How to use?
run
> $ ./server_threads

> $ ./clinet_threads [port]
>> Enter Your Name: Client1

> $ ./clinet_threads [port]
>> Enter Your Name: Client2

Client name cannot contain ":", "all", "bye", "exit"

Client says "exit", means dequeue from client list, other clients can reuse your name.
        says "bye", means go offline, name still cannot be used by others.


## Test
./server_threads 8888\
=== WELCOME TO THE CHATROOM ===\
\>

./client_threads 8888\
Please enter your name: c1\
=== WELCOME TO THE CHATROOM ===\
\>

### Direct message
\> hello > [client_name]

### Command
* exit
* bye
* list: list online/offline user
