#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bigint.h"

int int1024_cmp(int1024 * a, int1024 * b)
//return 0(a==b) or 1(a>b) or -1(a<b)
{
    int i = 0;
    while(i < MAX_SIZE){
        if(a->s[i] > b->s[i]) return 1;
        if(b->s[i] > a->s[i]) return -1;
    }
    return 0;
}

int int1024_charcmp(int1024 * a, unsigned char b){
    int i = MAX_SIZE - 1;
    if(a->s[i] != b) return 0;
    while(i-->0){
        if(a->s[i] != 0) return 0;
    }
    return 1;
}

int1024 * int1024_charmul(int1024 * a, unsigned char b){
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    int i = MAX_SIZE;
    int overflow = 0;
    while(i-->0){
        result->s[i] = (b * a->s[i] + overflow);
        overflow = (b * a->s[i] + overflow)/256;
    }
    result->overflow = overflow;
    return result;
}

int1024 * int1024_charmulto(int1024 * a, unsigned char b){
    int i = MAX_SIZE;
    int overflow = 0;
    unsigned char temp = 0;
    while(i-->0){
        temp = (b * a->s[i] + overflow);
        overflow = (b * a->s[i] + overflow)/256;
        a->s[i] = temp;
    }
    a->overflow = overflow;
    return a;
}

int1024 * int1024_charsubto(int1024 * a, unsigned char b){
    int1024 * tempb;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    tempb->s[MAX_SIZE-1] = b;
    int1024_subto(a, tempb);
    free(tempb);
    return a;
}

int1024 * int1024_charsub(int1024 * a, unsigned char b){
    int1024 * tempb, * result;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    tempb->s[MAX_SIZE-1] = b;
    result = int1024_sub(a, tempb);
    free(tempb);
    return result;
}

int1024 * int1024_moveleftto(int1024 * a, unsigned char b){
    if(b>=MAX_SIZE){
        memset(a, 0, sizeof(int1024));
        return a;
    }
    int i = 0;
    while(i<(MAX_SIZE - b)){
        a->s[i] = a->s[i+b];
        i++;
    }
    memset((a+MAX_SIZE-b), 0, b);
    return a;
}

int1024 * int1024_moverigntto(int1024 * a, unsigned char b){
    if(b>=MAX_SIZE){
        memset(a, 0, sizeof(int1024));
        return a;
    }
    int i = MAX_SIZE;
    while(i-->b){
        a->s[i] = a->s[i-b];
    }
    memset(a, 0, b);
    return a;
}

int1024 * int1024_add(int1024 * a, int1024 * b){
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    int i = MAX_SIZE, overflow = 0;
    while(i-->0){
        result->s[i] = a->s[i] + a->s[i] + overflow;
        overflow = (a->s[i] + a->s[i] + overflow)/256;
    }
    result->overflow = overflow;
    return result;
}

int1024 * int1024_sub(int1024 * a, int1024 * b){
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    int i = MAX_SIZE, overflow = 0;
    while(i-->0){
        if((a->s[i]) >= (b->s[i]+overflow)){
            result->s[i] = (a->s[i]) - (b->s[i]+overflow);
            overflow = 0;
        }
        else{
            result->s[i] = (a->s[i]) + 256 - (b->s[i]+overflow);
            overflow = 1;
        }
        result->s[i] = a->s[i] - a->s[i] + overflow;
        overflow = (a->s[i] + a->s[i])/256;
    }
    result->overflow = overflow;
    return result;
}

int1024 * int1024_subto(int1024 * a, int1024 * b){
    int1024 * result;
    result = int1024_sub(a, b);
    memcpy(a, result, sizeof(int1024));
    free(result);
    return a;
}

int1024 * int1024_mul(int1024 * a, int1024 * b){
    int1024 * result, * tempb;
    result = (int1024 *)calloc(1, sizeof(int1024));
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    memcpy(tempb, b, sizeof(int1024));

    int i = MAX_SIZE, overflow = 0;
    unsigned char temp;
    while(i-->0){
        if(i<127) int1024_moveleftto(tempb, 1);
        overflow = 0;
        int j = MAX_SIZE;
        while(j-->0){
            temp = result->s[i] + a->s[i] * tempb->s[i] + overflow;
            overflow = (result->s[i] + a->s[i] * tempb->s[i] + overflow)/256;
            result->s[i] = temp;
        }
        if(overflow > 0) result->overflow = 1;
    }
    result->overflow = overflow;
    free(tempb);
    return result;

}

int1024 * int1024_multo(int1024 * a, int1024 * b){
    int1024 * result;
    result = int1024_mul(a, b);
    memcpy(a, result, sizeof(int1024));
    free(result);
    return a;
}

int1024 * int1024_pow(int1024 * a, int1024 * b, int1024 * c)
//(a^b)%c, a and b will be changed
//a will be return value, b will be 1;
{
    while(int1024_cmp(a, c) > 0){// a = a%c
        
    }
    if(int1024_charcmp(b, 0)){
        memset(a, 0, sizeof(int1024));
        return a;
    }
    if(((int)b == 1) || int1024_charcmp(b, 1)){
        return a;
    }
    if((b->s[MAX_SIZE - 1]%2) > 0){
        int1024 * tempa;
        tempa = (int1024 *)calloc(1, sizeof(int1024));
        memcpy(tempa, a, sizeof(int1024));
        int1024_pow(a, int1024_charsubto(b, 1), c);
        int1024_multo(a, tempa);
        free(tempa);
        return int1024_pow(a, (int1024 *)1, c);
    }
    else{
        int1024 * tempa;
        tempa = int1024_mul(a, a);
        memcpy(a, tempa, sizeof(int1024));
        free(tempa);
        int1024_moverigntto(b, 1);
        return int1024_pow(a, b, c);
    }
}

int1024 * int1024_get_prime(int len){
    if(len > MAX_SIZE){
        puts("len of random to long.");
        exit(0);
    }
    int1024 * s;
    while(1){
        s = int1024_random(len);
        if(int1024_rabin(s)){
            return s;
        }
        free(s);
    }


}

int int1024_rabin(int1024 * n){
    int i = TEST_TIMES;
    int1024 * a, *n_1;
    n_1 = int1024_charsub(n ,1);
    while(i-->0){
        a = int1024_random(128);
        if(int1024_cmp(a, n) > 0){
            int1024_pow(a, (int1024 *)1, n);
        }
        int q = 0;
        int1024 * r;

        r = (int1024 *)calloc(1, sizeof(int1024));
        memcpy(r, n, sizeof(int1024));
        int1024_charsubto(r, 1);
        while(1){
            int1024 * tempa;
            tempa = (int1024 *)calloc(1, sizeof(int1024));
            memcpy(tempa, a, sizeof(int1024));
            if(int1024_charcmp(int1024_pow(tempa, r, n), 1)){
                q++;
                int1024_moveleftto(r, 1);
                free(tempa);
                if(r->s[MAX_SIZE - 1]%2 > 0){
                    break;
                }
                continue;
            }
            memcpy(tempa, a, sizeof(int1024));
            if(!int1024_cmp(int1024_pow(tempa, r, n), n_1)){
                free(tempa);
                break;
            }
            else{
                free(tempa);
                free(n_1);
                free(r);
                free(a);
                return  0;
            }

        }        
    }
    return 1;
}

int1024 * int1024_random(int len){
    if(len > MAX_SIZE){
        puts("random to long.");
        exit(0);
    }
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    FILE * rfd;
    rfd = fopen("/dev/urandom", O_RDONLY);
    fread(result->s, 1, len, rfd);
    fclose(rfd);
    return result;
}

int main(int argc, char ** args){
    int1024 * a, * b;
    long long int start = clock();
    a = (int1024 *)calloc(1, sizeof(int1024));
    b = (int1024 *)calloc(1, sizeof(int1024));
    for(int i=0; i<32768; i++){
        int1024 *r;
        r = int1024_add(a, b);
        //free(r);
    }
    long long int end = clock();
    printf("%lld", end-start);
}