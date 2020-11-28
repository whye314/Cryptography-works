
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

int1024 * get_prime_random(int len);//windows cann't create a ditigal random convenience.

int create_key(int key_len){
    int1024 * p, * q, * n;
    p = int1024_get_prime(64);
    q = int1024_get_prime(64);
    n = int1024_mul(q, p);
    

}
