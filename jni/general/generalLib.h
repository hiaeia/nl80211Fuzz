#ifndef __GENERAL_LIB__
#define __GENERAL_LIB__
enum {
    E,
    I,
    D,
};

#define gfuzz(x) (grand(sizeof((x)), (&x)))
void m_printmem(void *ptr, unsigned int len);
void glog(int type, char *fmt, ...);
void *m_brand(unsigned int len);//binary string
void *m_srand(unsigned int len);// string, end with '\0'
unsigned int m_urand(unsigned int max);
#endif
