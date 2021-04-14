#include <stdio.h>
#include <stdlib.h>

int main() {
    int a = 124, bin = 0;
    int rem, tmp = 1;
    char h[2];

    // while(a != 0) {
    //     rem = a%2;
    //     a = a/2;
    //     bin = bin + rem*tmp;
    //     tmp = tmp*10;
    // }
    sprintf(h, "%2x", a);
    puts(h);

    return 0;
}
