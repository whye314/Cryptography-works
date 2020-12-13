#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
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
        i++;
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

int1024 * int1024_chardivto(int1024 * a, unsigned char b){
    int1024 * tempb;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    tempb->s[MAX_SIZE-1] = b;
    int1024_divto(a, tempb);
    free(tempb);
    return a;
}

int1024 * int1024_chardiv(int1024 * a, unsigned char b){
    int1024 * tempb, * result;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    tempb->s[MAX_SIZE-1] = b;
    result = int1024_div(a, tempb);
    free(tempb);
    return result;
}

int1024 * int1024_charadd(int1024 * a, unsigned char b){
    int1024 * tempb, * result;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    tempb->s[MAX_SIZE-1] = b;
    result = int1024_add(a, tempb);
    free(tempb);
    return result;
}

int1024 * int1024_charaddto(int1024 * a, unsigned char b){
    int1024 * tempb;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    tempb->s[MAX_SIZE-1] = b;
    int1024_addto(a, tempb);
    free(tempb);
    return a;
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
    memset((a->s+MAX_SIZE-b), 0, b);
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
    memset(a->s, 0, b);
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

int1024 * int1024_addto(int1024 * a, int1024 * b){
    int1024 * result;
    result = int1024_add(a, b);
    memcpy(a, result, sizeof(int1024));
    free(result);
    return a;
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
        // result->s[i] = a->s[i] - a->s[i] + overflow;
        // overflow = (a->s[i] + a->s[i])/256;
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

int1024 * int1024_div(int1024 * a, int1024 * b){
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
     int1024 * ta;
     ta = (int1024 *)calloc(1, sizeof(int1024));
    // memcpy(ta, a, sizeof(int1024));
     int1024 * tempa;
     tempa = (int1024 *)calloc(1, sizeof(int1024));
    // while(int1024_cmp(ta, b) > 0){
    //     memcpy(tempa, ta, sizeof(int1024));
    //     if(int1024_cmp(int1024_moverigntto(tempa, 1), b) > 0){
    //         int1024 * tempb;
    //         memcpy(tempb, b, sizeof(int1024));
    //         int1024_moveleftto(tempb, 1);
    //         int1024_subto(ta, tempb);
    //         int1024_charaddto(result, 128);
    //         int1024_charaddto(result, 128);
    //         free(tempb);
    //     }
    //     int1024_subto(ta, b);
    //     int1024_charaddto(result, 1);
    // }
    // free(tempa);
    int i = MAX_SIZE;
    int1024 * tempb;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    while(i-->0){
        memcpy(tempa, ta, sizeof(int1024));
        int1024_moverigntto(tempa, i);
        memcpy(tempb, b, sizeof(int1024));
        int1024_moveleftto(tempb, i);
        while(int1024_cmp(tempa, b) >= 0){
            int1024_subto(ta, tempb);
            int1024_subto(tempa, b);
            result->s[MAX_SIZE-i-1] += 1;
        }
        
    }
    free(tempb);

    free(ta);
    return result;
}

int1024 * int1024_divto(int1024 * a, int1024 * b){
    int1024 * result;
    result = int1024_div(a, b);
    memcpy(a, result, sizeof(int1024));
    free(result);
    return a;
}

int1024 * int1024_mulmod(int1024 * a, int1024 * b, int1024 * c){
    
}

int1024 * int1024_powto(int1024 * a, int1024 * b, int1024 * c)
//(a^b)%c, a and b will be changed
//a will be return value, b will be 1;
{

    // while(int1024_cmp(a, c) >= 0){
    //     int i = MAX_SIZE;
    //     while(i-->0){
    //         memcpy(tempa, a, sizeof(int1024));
    //         if(int1024_cmp(int1024_moverigntto(tempa, i), c) > 0){
    //             int1024 * tempc;
    //             tempc = (int1024 *)calloc(1, sizeof(int1024));
    //             memcpy(tempc, c, sizeof(int1024));
    //             int1024_moveleftto(tempc, i);
    //             int1024_subto(a, tempc);
                
    //             free(tempc);
    //         }
    //     }
    // }
    // while(int1024_cmp(a, c) >= 0){// a = a%c

    //     memcpy(tempa, a, sizeof(int1024));
    //     if(int1024_cmp(int1024_moverigntto(tempa, 1), c) > 0){
    //         int1024 * tempc;
    //         tempc = (int1024 *)calloc(1, sizeof(int1024));
    //         memcpy(tempc, c, sizeof(int1024));
    //         int1024_moveleftto(tempc, 1);
    //         int1024_subto(a, tempc);
            
    //         free(tempc);
    //     }
    //     else{
    //         int1024_subto(a, c);
    //     }
        
    // }
    int1024 * tempa;
    if(int1024_cmp(a, c) >= 0){   //a=a%c     
        
        tempa = (int1024 *)calloc(1, sizeof(int1024));
        int i = MAX_SIZE;
        int1024 * tempc;
        tempc = (int1024 *)calloc(1, sizeof(int1024));
        while(i-->0){
            memcpy(tempa, a, sizeof(int1024));
            int1024_moverigntto(tempa, i);
            memcpy(tempc, c, sizeof(int1024));
            int1024_moveleftto(tempc, i);
            while(int1024_cmp(tempa, c) >= 0){
                int1024_subto(a, tempc);
                int1024_subto(tempa, c);
            }
            
        }
        free(tempc);

        free(tempa);
    }
    if(((long long)b == 1) || int1024_charcmp(b, 1)){
        return a;
    }
    if(int1024_charcmp(b, 0)){
        memset(a, 0, sizeof(int1024));
        return a;
    }

    // if((b->s[MAX_SIZE - 1]%2) > 0){
    //     //int1024 * tempa;
    //     tempa = (int1024 *)calloc(1, sizeof(int1024));
    //     memcpy(tempa, a, sizeof(int1024));
    //     int1024_powto(a, int1024_charsubto(b, 1), c);
    //     int1024_multo(a, tempa);
    //     free(tempa);
    //     return int1024_powto(a, (int1024 *)1, c);
    // }
    // else{
    //     //int1024 * tempa;
    //     tempa = int1024_mul(a, a);
    //     memcpy(a, tempa, sizeof(int1024));
    //     free(tempa);
    //     int i = 0;
    //     unsigned char over = 0;
    //     while(i<MAX_SIZE){
    //         unsigned temp = b->s[i]/2 + over<<7;
    //         over = b->s[i]%2;
    //         b->s[i] = temp;
    //         i++;
    //     }
    //     return int1024_powto(a, b, c);
    // }


    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    result->s[127] = 1;

    while(int1024_charcmp(b, 0)){
        if((b->s[127]%2) == 1){
            result = int1024_mulmod(result, a, c);
        }
        a = (a, a, c);

        int1024_chardivto(b,2);
    }
}

int1024 * int1024_get_prime(int len){
    if(len > MAX_SIZE*8){
        puts("len of random too long.");
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
        a = int1024_random(1024);
        if(int1024_cmp(a, n) > 0){
            int1024_powto(a, (int1024 *)1, n);
        }
        int q = 0;
        int1024 * r;

        // r = (int1024 *)calloc(1, sizeof(int1024));
        // memcpy(r, n, sizeof(int1024));
        r = int1024_charsub(n, 1);
        // int1024_charsubto(r, 1);
        //while(1){
            int1024 * tempa;
            tempa = (int1024 *)calloc(1, sizeof(int1024));
            memcpy(tempa, a, sizeof(int1024));
            int1024 * tempr;
            tempr = (int1024 *)calloc(1, sizeof(int1024));
            memcpy(tempr, r, sizeof(int1024));
            while(int1024_charcmp(int1024_powto(tempa, tempr, n), 1)){
                q++;
                //int1024_moveleftto(r, 1);
                int1024_chardivto(r, 2);
                //free(tempa);
                if(r->s[MAX_SIZE - 1]%2 > 0){
                    break;
                }
                memcpy(tempr, r, sizeof(int1024));
                memcpy(tempa, a, sizeof(int1024));
                continue;
            }
            memcpy(tempa, a, sizeof(int1024));
            if(!int1024_cmp(int1024_powto(tempa, r, n), n_1)){
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

        //}        
    }
    return 1;
}

int1024 * int1024_random(int len){
    if(len > MAX_SIZE*8){
        puts("random too long.");
        exit(0);
    }
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    FILE * rfd;
    rfd = fopen("/dev/urandom", "r");
    fread((result->s+(MAX_SIZE-len/8)), 1, len/8, rfd);
    fclose(rfd);
    return result;
}

// int1024 * int1024_exgcd(int1024 * a, int1024 * b){// return ax=1(mod b)
//     int1024 * tempa;
//     tempa = (int1024 *)calloc(1, sizeof(int1024));
//     memcpy(tempa, a, sizeof(int1024));
//     if(int1024_cmp(tempa, b)>=0){
//         int1024_powto(tempa, (int1024 *)1, b);
//     }
//     int1024 * x[3];
//     x[0] = int1024_charaddto((int1024 *)calloc(1, sizeof(int1024)), 1);
//     x[1] = (int1024 *)calloc(1, sizeof(int1024));
//     x[2] = (int1024 *)calloc(1, sizeof(int1024));
//     memcpy(x[2], b, sizeof(int1024));
//     int1024 * y[3];
//     y[0] = (int1024 *)calloc(1, sizeof(int1024));
//     y[1] = int1024_charaddto((int1024 *)calloc(1, sizeof(int1024)), 1);
//     y[2] = (int1024 *)calloc(1, sizeof(int1024));
//     memcpy(y[2], tempa, sizeof(int1024));
//     free(tempa);
//     while(1){
//         if(int1024_charcmp(y[2], 0)){
//             int i = 3;
//             while(i-->0){
//                 free(x[i]);
//                 free(y[i]);
//             }
//             return 0;
//         } 
//         if(int1024_charcmp(y[2], 1)){
//             int i = 3;
//             while(i-->0){
//                 if(i==1) free(x[i]);
//                 else{
//                     free(x[i]);
//                     free(y[i]);
//                 }
//             }
//             return y[1];
//         }
//         int1024 * q;
//         q = int1024_div(x[2], y[2]);
//         int i = 3;
//         //int1024 * t[3];
//         while(i-->3){
//             int1024 * qy;
//             qy = int1024_mul(q, y[i]);
//             int1024 * t;
//             t = int1024_sub(x[i], qy);
//             free(x[i]);
//             free(qy);
//             x[i] = y[i];
//             y[i] = t;
//         }
//         free(q);
//     }
// }

int1024 * int1024_exgcd(int1024 * a, int1024 * b){
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    int p[2];
    pipe(p);
    char s[3];
    s[0] = p[0];
    s[1] = p[1];
    s[2] = 0;
    char *sa[] = {"exgcd.py", s, NULL};
    pid_t pid;
    pid = fork();
    if(pid == 0){
        execve("exgcd.py", sa, NULL);
    }
    write(p[1], a->s, MAX_SIZE);
    write(p[1], b->s, MAX_SIZE);
    //kill(pid, SIGUSR2);
    waitpid(pid, NULL, 0);
    read(p[0], result->s, MAX_SIZE);
    close(p[0]);
    close(p[1]);
    return result;

}


// int main(int argc, char ** args){
//     int1024 * a, * b;
//     long long int start = clock();
//     a = (int1024 *)calloc(1, sizeof(int1024));
//     b = (int1024 *)calloc(1, sizeof(int1024));
//     for(int i=0; i<32768; i++){
//         int1024 *r;
//         r = int1024_add(a, b);
//         //free(r);
//     }
//     long long int end = clock();
//     printf("%lld", end-start);
// }
// int main(int argc, char ** args){
//     int1024 * p, * q, * m;
//     p = (int1024 *)calloc(1, sizeof(int1024));
//     q = (int1024 *)calloc(1, sizeof(int1024));
//     p->s[127] = 5;
//     q->s[127] = 7;
//     m = int1024_exgcd(p, q);
//     printf("%d", m->s[127]);
//     return 0;
// }
int main(int argc, char ** args){
    int1024 * p, * q, * m;
    p = (int1024 *)calloc(1, sizeof(int1024));
    q = (int1024 *)calloc(1, sizeof(int1024));
    p->s[127] = 5;
    q->s[127] = 7;
    printf("%d", int1024_rabin(p));
}