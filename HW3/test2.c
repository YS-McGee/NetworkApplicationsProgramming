#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define LENGTH 2048

int main () {
    char *src = "123";
	char dest[LENGTH];

    while(1) {
        printf("\n");
        //fgets(src, LENGTH, stdin);
        strcat(dest, src);
	    printf(dest);
        printf("\n\n");
        sleep(1);
    }

	return 0;
}