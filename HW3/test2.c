#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main () {
    int id = fork();
    printf("Hello from %d\n", id);
}