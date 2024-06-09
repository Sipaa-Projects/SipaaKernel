#include <sipaa/libc/string.h>
#include <sipaa/heap.h>

#define DICT_LEN 256

int *CreateDelimitationDictionnary(char *delim)
{
    int *d = (int*)KHeap_Allocate(sizeof(int)*DICT_LEN);
    memset((void*)d, 0, sizeof(int)*DICT_LEN);

    int i;
    for(i=0; i< StringLength(delim); i++) {
        d[delim[i]] = 1;
    }
    return d;
}

char *TokenizeString(char *str, char *delim)
{
    static char *last, *to_free;
    int *deli_dict = CreateDelimitationDictionnary(delim);

    if(!deli_dict) {
        return NULL;
    }

    if(str) {
        last = (char*)KHeap_Allocate(StringLength(str)+1);
        if(!last) {
            KHeap_Free(deli_dict);
        }
        to_free = last;
        CopyString(last, str);
    }

    while(deli_dict[*last] && *last != '\0') {
        last++;
    }
    str = last;
    if(*last == '\0') {
        KHeap_Free(deli_dict);
        KHeap_Free(to_free);
        return NULL;
    }
    while (*last != '\0' && !deli_dict[*last]) {
        last++;
    }

    *last = '\0';
    last++;

    KHeap_Free(deli_dict);
    return str;
}