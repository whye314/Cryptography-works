#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "des.h"


int main(int argc, char ** args){
    if(argc < 2){
        puts("message path error");
        puts("./des.exe {mseeagefilename/ciphertextfilename} {keyfilename} {targetfilename} {operation}");
        puts("operation must be encrypto(e) or decrypto(d)");
        exit(0);
    }
    if(argc < 3){
        puts("key path error");
        puts("./des.exe {mseeagefilename/ciphertextfilename} {keyfilename} {targetfilename} {operation}");
        puts("operation must be encrypto(e) or decrypto(d)");
        exit(0);
    }
    if(argc < 4){
        puts("key path error");
        puts("./des.exe {mseeagefilename/ciphertextfilename} {keyfilename} {targetfilename} {operation}");
        puts("operation must be encrypto(e) or decrypto(d)");
        exit(0);
    }
    if(argc < 5){
        puts("error operation");
        puts("./des.exe {mseeagefilename/ciphertextfilename} {keyfilename} {targetfilename} {operation}");
        puts("operation must be encrypto(e) or decrypto(d)");
        exit(0);
    }
    char * mess_path, * key_path, * target_path, * operation;
    mess_path = args[1];
    key_path = args[2];
    target_path = args[3];
    operation = args[4];
    int ED;
    if(!memcmp(operation, "e", 2) || !memcmp(operation, "encrypto", 9))
        ED = ENCODE;
    else if(!memcmp(operation, "d", 2) || !memcmp(operation, "decrypto", 9))
        ED = DECODE;

    FILE * mess_fd, * key_fd, * target_fd;
    mess_fd = fopen(mess_path, "r");
    key_fd = fopen(key_path, "r");
    
    
    int64 * head;
    head = (int64 *)calloc(1, sizeof(int64));
    head->final = head;
    char * mess, * key;
    mess = (char *)calloc(1, 9);
    key = (char *)calloc(1, 9);
    fread(key, 1, 8, key_fd);
    fclose(key_fd);
    while(1){
        memset(mess, 0, 8);
        if(fread(mess, 1, 8, mess_fd) == 0)
            break;
        int64 * node;
        node = des(mess, key, ED);
        head->final->next = node;
        head->final = node;
    }
    fclose(mess_fd);
    head = head->next;


    #if _WIN64  //识别windows平台
        target_fd = fopen(target_path, "wb");//windows必须是wb模式打开二进制文件，否则遇到0x0a时会被作为换行，被自动替换为0x0d0a
    #elif __linux__  //识别linux平台
        target_fd = fopen(target_path, "w");
    #endif


    
    unsigned char * tmps;
    tmps = (unsigned char *)calloc(1, 8 * sizeof(unsigned char));
    while(head!=NULL){
        memset(tmps, 0, 8);
        int i = 0, j = 0;
        for(i = 0; i < 8; i++){
            for(j = 0; j < 8; j++){
                //tmps[i] |= ((unsigned char)head->val[8*i+j]<<j);
                tmps[i] |= ((unsigned char)head->val[8*i+j]<<(7 - j));
            }
        }
        fwrite(tmps, 1, 8, target_fd);
        head = head->next;
    }
    free(tmps);
    fclose(target_fd);
    return 0;
        


}
    