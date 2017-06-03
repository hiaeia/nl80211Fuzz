#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include "generalLib.h"

#define PAGE_SIZE 4096
#define CACHE_SIZE (PAGE_SIZE * 64)
//#define GENERAL_DEBUG

struct random_cache{
    unsigned int rem;
    unsigned char buf[CACHE_SIZE];
    union {
        void *ptr;
        unsigned long *u64_ptr;
    };
};

static struct random_cache cache = {
    .rem = 0,
    .ptr = &cache.buf,
};

void rand_next(unsigned int len){
    int act = (len + 7)/8;
    cache.u64_ptr += act;
    cache.rem -= (act * 8);
}

void init_random_cache(){
    int fd;
    unsigned int remained, len;

    fd = open("/dev/urandom", 0);
    if (fd < 0){
        glog(E, "open /dev/urandom failed.\n");
        return;
    }

    remained = CACHE_SIZE;
    while (remained){
        len = read(fd, (void *)(cache.buf + CACHE_SIZE - remained), remained);
        remained -= len;
    }

    close(fd);
    cache.rem = CACHE_SIZE;
    cache.ptr = &cache.buf;
    glog(D, "buf has inited in [%p, %p)\n", &(cache.buf[0]), &(cache.buf[CACHE_SIZE]));
    //m_printmem(cache.ptr, 16);
}

void m_printmem(void *ptr, unsigned int len){
    unsigned char *p = (unsigned char *)ptr;
    int i;

    for(i = 0; i < len; ++i, ++p){
        printf("%02x ", *p);
        if (i % 8 == 7)
            printf("\n");
    }
    if(i % 8 == 4)
        printf("\n");
}

void glog(int type, char *fmt, ...){
    va_list args;
    FILE *out;
    char nfmt[256];

    if (strlen(fmt) > 128){
        fprintf(stderr, "[-]ERROR:fmt is too long, should be less than 128!\n");
        return;
    }
    memset(nfmt, 0, 256);

    va_start(args, fmt);//point to the next of fmt
    
    switch (type){
        case D:
#ifdef GENERAL_DEBUG
            out = stdout;
            strcpy(nfmt, "[*]Debug:");
            strcpy((char *)(nfmt + 9), fmt);
            break;
#else
            return;
#endif
        case E:
            out = stderr;
            strcpy(nfmt, "[-]Error:");
            strcpy((char *)(nfmt + 9), fmt);
            break;
        case I:
            strcpy(nfmt, "[+]Info:");
            strcpy((char *)(nfmt + 8), fmt);
        default:
            out = stdout;
    }

    vfprintf(out, nfmt, args);
    va_end(args);//point to NULL
}

void *m_brand(unsigned int len){
    void *ptr;
    if (len >= CACHE_SIZE) {
        glog(E, "len is too big to alloc\n");
        return NULL;
    }
    
    if (cache.rem < len)
        init_random_cache();
    
    ptr = cache.ptr;
    //m_printmem((char *)ptr, 16);
    rand_next(len);
    return ptr;
}

void *m_srand(unsigned int len){
    void *ptr;
    if (len + 1 >= CACHE_SIZE) {
        glog(E, "len is too big to alloc\n");
        return NULL;
    }

    if (cache.rem < len + 1)
        init_random_cache();
    
    ptr = cache.ptr;
    *((char *)(cache.ptr) + len) = '\0';
    rand_next(len + 1);
    return ptr;
}

unsigned int m_urand(unsigned int max){
    unsigned int ret;    

    if (cache.rem < 8){
        init_random_cache();
    }
    
    ret = *((unsigned int *)(cache.ptr));
    rand_next(4);
    if (max == 0)
        return ret;
    else
        return ret % (max + 1);
}
