#include <stdio.h>



int main(int argc, char ** args){
    int i = argc;
    while(i-->0){
        printf("%d ", i);
        puts(args[i]);
    }
}