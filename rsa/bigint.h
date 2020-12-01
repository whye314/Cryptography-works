#define MAX_SIZE 128
#define TEST_TIMES 10

#define MIN(a, b) a>b?b:a
#define MAX(a, b) a>b?a:b
#define OVERFLOWUCHAR(a, b) (a+b)>255?1:0

struct _bigint_struct{
    unsigned char s[MAX_SIZE];
    unsigned overflow;
};

typedef struct _bigint_struct int1024;



int int1024_cmp(int1024 * a, int1024 * b);
int int1024_charcmp(int1024 * a, unsigned char b);
int1024 * int1024_charmul(int1024 * a, unsigned char b);
int1024 * int1024_charmulto(int1024 * a, unsigned char b);
int1024 * int1024_charsubto(int1024 * a, unsigned char b);
int1024 * int1024_charsub(int1024 * a, unsigned char b);
int1024 * int1024_charadd(int1024 * a, unsigned char b);
int1024 * int1024_charaddto(int1024 * a, unsigned char b);
int1024 * int1024_moveleftto(int1024 * a, unsigned char b);
int1024 * int1024_moverigntto(int1024 * a, unsigned char b);
int1024 * int1024_add(int1024 * a, int1024 * b);
int1024 * int1024_addto(int1024 * a, int1024 * b);
int1024 * int1024_sub(int1024 * a, int1024 * b);
int1024 * int1024_subto(int1024 * a, int1024 * b);
int1024 * int1024_mul(int1024 * a, int1024 * b);
int1024 * int1024_multo(int1024 * a, int1024 * b);
int1024 * int1024_div(int1024 * a, int1024 * b);
int1024 * int1024_divto(int1024 * a, int1024 * b);
int1024 * int1024_powto(int1024 * a, int1024 * b, int1024 * c);
int1024 * int1024_get_prime(int len);
int int1024_rabin(int1024 * n);
int1024 * int1024_random(int len);
int1024 * int1024_exgcd(int1024 * a, int1024 * b);
