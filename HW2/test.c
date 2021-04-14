#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compress(FILE *fp, FILE *huff_fp) {

    int k, h = 0;
    int c;
    int b[8], bit;
    char hex[2];
    
    while( (c = fgetc(fp) ) != EOF) {
        // for(k = 1 << 7; k > 0; k = k / 2, ++h) {
        //     (c & k) ? (b[h] = 1) : (b[h] = 0);
        // }
        // h = 0;
        sprintf(hex, "%2x", c);
        //printf("%c ", c);
        //puts(hex);
        fwrite(hex, sizeof(char), 2, huff_fp);
    }
    //printf("\n");
}

void decompress(FILE *fp, FILE *new_fp) {

    int count, a = 0, i, base = 1, num[1];
    char c[2];

    while(1) {
        if( ( count = fread(&c, sizeof(char), 2, fp) ) == 0) {
            printf("\n");
            return;
        }
        for(i = 1; i >= 0; --i) {
            if(c[i] >= '0' && c[i] <= '9') {
                a += (c[i] - 48) * base;
                base *= 16;
            }
            else if(c[i] >= 'A' && c[i] <= 'F') {
                a += (c[i] - 55 ) * base;
                base *= 16;
            }
            else if(c[i] >= 'a' && c[i] <= 'f') {
                a += (c[i] -87) * base;
                base *= 16;
            }
        }
        fprintf(new_fp, "%c", a);
        base = 1;
        a = 0;
    }
    printf("\n");
}

int main() {

    FILE *fp, *huff_fp, *new_fp;

    fp = fopen("THY_ART_IS_MURDER.mp3", "r");
    huff_fp = fopen("compressed", "w");

    compress(fp, huff_fp);
    fclose(huff_fp);

    huff_fp = fopen("compressed", "r");
    new_fp = fopen("decomp", "w");
    decompress(huff_fp, new_fp);

    fclose(fp);
    fclose(huff_fp);
    fclose(new_fp);
}
