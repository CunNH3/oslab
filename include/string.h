#ifndef INC_STRING_H
#define INC_STRING_H

#include "types.h"

char *itoa(int);
void memcpy(void *, const void *, size_t);
void memset(void *, int, size_t);
size_t strlen(const char *);
void strcpy(char *d, const char *s);

#endif 
