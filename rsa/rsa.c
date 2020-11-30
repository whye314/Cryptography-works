
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bigint.h"
#include "rsa.h"


int1024 ** rsa_create_key(int key_len){// return [e, n, d]
    int1024 * p, * q, * n;
    p = int1024_get_prime(key_len/16);
    q = int1024_get_prime(key_len/16);
    n = int1024_mul(q, p);
    int1024 * p_1, * q_1;
    p_1 = int1024_charsub(p, 1);
    q_1 = int1024_charsub(q, 1);
    int1024 * psi_n;
    psi_n = int1024_mul(q_1, p_1);
    free(q_1);free(p_1);
    int1024 * e;
    e = int1024_get_prime(key_len/8);
    int1024_pow(e, 1, psi_n);
    int1024 * d;
    d = int1024_exgcd(e, psi_n);
    free(p);
    free(q);
    free(psi_n);
    int1024 ** result;
    result = (int1024 **)calloc(1, sizeof(int1024 *) * 3);
    result[0] = e;
    result[1] = n;
    result[2] = d;
    return result;

}

int1024 * rsa(int1024 * a, int1024 * b, int1024 * n){//ciphertext/message, key, n
    int1024 * result;
    result = (int1024 *)calloc(1, sizeof(int1024));
    memcpy(result, a ,sizeof(int1024));
    int1024 * tempb;
    tempb = (int1024 *)calloc(1, sizeof(int1024));
    memcpy(tempb, b, sizeof(int1024));
    int1024_powto(result, tempb, n);
    free(tempb);
    return result;
}
