#ifndef MYALLOC_H
#define MYALLOC_H

struct block {
    int size;            // Size in bytes
    int in_use;          // Boolean
    struct block *next;
};

void *myalloc(int size);

void myfree(void *p);



#endif