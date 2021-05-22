#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define LENGTH 2048

int main () {
    char string[50] = "msg >c1> c2";
   // Extract the first token
   char * token1, *token2;
   token1 = strtok(string, " > ");
   // loop through the string to extract all other tokens
   
    token2 = strtok(NULL, " > ");
    printf("token1=%s=\ntoken2=%s=\n", token1, token2);

    return 0;
}