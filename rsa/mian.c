#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "rsa.h"




int main(int argc, char ** args){
    if(!(argc == 3 || argc == 4)){
        printf("arguments error.");
        alert(args[0]);
        exit(0);
    }
    
    
if(argc == 3){
    create_key(args[1], args[2]);
    return 0;
}
if(argc == 4){
    char * sourcefile, * targetfile, * keyfile;
    sourcefile = args[1];
    keyfile = args[2];
    targetfile = args[3];
    FILE * sourcefd, * targetfd, * keyfd;
    if((sourcefd = fopen(sourcefile, "rb"))<0){
        printf("messagefilename/cipherfilename don't exist.");
        alert(args[0]);
        exit(0);
    }
    if((keyfd = fopen(keyfile, "rb"))<0){
        printf("keyfilename don't exist.");
        alert(args[0]);
        fclose(sourcefd);
        exit(0);
    }
    targetfd = fopen(targetfile, "wb");

    int len = 0;
    int1024 * eord, * n;
    eord = (int1024 *)calloc(1, sizeof(int1024));
    n = (int1024 *)calloc(1, sizeof(int1024));
    sscanf("%ud\r\n\r\n", len);
    fread((eord->s+(MAX_SIZE-len)/8), 1, len/8, keyfd);
    fread(NULL, 1, 2, keyfd);
    fread((n->s+(1024-len)/8), 1, len/8, keyfd);
    fclose(keyfd);


    int1024 * buf, * result;
    buf = (int1024 *)calloc(1, sizeof(int1024));

    while(fread((buf->s + (MAX_SIZE - DEFAULT_GROUP_LEN)/8), 1, DEFAULT_GROUP_LEN/8, sourcefd) > 0){
        result = rsa(buf, eord, n);
        fwrite((result->s+(MAX_SIZE-len)/8), 1, DEFAULT_GROUP_LEN/8, targetfd);        
        free(result);
        memset(buf->s, 0, DEFAULT_GROUP_LEN/8);

    }
    fclose(sourcefd);
    fclose(targetfd);
    puts("successfully.");
    return 0;
}

    
}
    

int alert(char * n){
    printf("something error.\n");
    printf("if you want to create a key: %s {publicfilename} {privatrfilename}\n");
    printf("or you want to en/decrypto: %s {messagefilename/cipherfilename} {publickeyfilname/privatekeyfilename} {targetfilename}\n");
    //printf("operation must be encrypto(e) or decrypto(d)");
    return 0;
}


int create_key(char * public_file, char * private_file){
    int1024 * e, * d, * n;
    int1024 ** k;
    k = rsa_create_key(DEFAULT_KEY_LEN);
    e = k[0];
    n = k[1];
    d = k[2];
    FILE * pub, * pri;
    pub = fopen(public_file, "wb");
    pri = fopen(private_file, "wb");
    unsigned int len = DEFAULT_KEY_LEN;
    fprintf(pub, "%ud\r\n\r\n%.128s\r\n%.128s\r\n", len, e->s, n->s);
    fprintf(pri, "%ud\r\n\r\n%.128s\r\n%.128s\r\n", len, d->s, n->s);
    fclose(pub);
    fclose(pri);
    return 0;
}