#include <stdio.h>
#include <stdlib.h>


int main(int argc, char ** args){
    puts("6");
    unsigned long long  n;
    puts("6");
    n  = 0x0123456789abcdef;
    puts("8");
    FILE * f;
    f = fopen("mess.txt", "w");
    puts("10");
    fwrite((char *)&n, 1, 8, f);
    fclose(f);
    //return 0;
    puts("12");

    //unsigned long long * n;
    n  = 0x133457799bbcdff1;
    //FILE * f;
    f = fopen("key.txt", "w");
    fwrite((char *)&n, 1, 8, f);
    fclose(f);
    return 0;
}