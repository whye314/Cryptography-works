

#define ENCODE 0
#define DECODE 1

#define KEY_REPLACE1_LEN 56
#define KEY_REPLACE2_LEN 48
#define IP_LEN 64
#define IP_1_LEN 64
#define E_LEN 48
#define P_LEN 32

#define XOR(a, b) a==b?0:1




struct _int64_struct{
    union{
        char val[64];
        struct _int64_struct * final;
    };
    union{
        //int len;
        struct _int64_struct * next;
    };
    
};

typedef struct _int64_struct int64;

int64 * des(char * text, char * key, int ED);


