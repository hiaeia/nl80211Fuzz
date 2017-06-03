#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include "../general/generalLib.h"

struct test{
    int a;
    long b;
    void *ptr;
};

int  main(int argc, char**argv){
    struct test t;    
    int a = m_urand(255);
    printf("%d\n", a);

    a = m_urand(255);
    printf("%d\n", a);
    
    memcpy(&t, m_brand(sizeof(struct test)), sizeof(struct test));
    printf("%d\n", t.a);
    printf("%ld\n", t.b);
    printf("%p\n", t.ptr);
    return 0;
}
