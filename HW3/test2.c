#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main () {
    char *s1 = ": ";
    char *s2 = "exit";
    char *s3 = ":";

    if(strstr(s1, s2) || strstr(s1, s3)) {
        printf("Contains\n");
    }
}