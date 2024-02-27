#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "myalloc.h"
//Daniel Lounsbury
#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block *head = NULL;

void *myalloc(int size){
  if (head == NULL){
void *heap = mmap(NULL, 1024, PROT_READ|PROT_WRITE,
                  MAP_ANON|MAP_PRIVATE, -1, 0);
    head = (struct block *)heap;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
    head->next = NULL;
}

struct block *current = head;
int remaining_space;
while(current != NULL){
if(!current->in_use && current->size >= size){
  remaining_space = current->size - PADDED_SIZE(size);
  if(remaining_space >= (int)PADDED_SIZE(sizeof(struct block))+ALIGNMENT){
    struct block *new_block = (struct block *)PTR_OFFSET(current, PADDED_SIZE(size));
    new_block->size = remaining_space - PADDED_SIZE(sizeof(struct block));
    new_block->in_use = 0;
    new_block->next = current->next;

    current->size = PADDED_SIZE(size);
    current->next = new_block;

  }
  
  current->in_use = 1;
  return PTR_OFFSET(current, PADDED_SIZE(sizeof(struct block)));
}
current = current->next;


}
return NULL;
}

void coalesce(struct block *head)
{
        struct block *current = head;
        while(current->next!=NULL)
    {
        if(current->in_use==0 && current->next->in_use==0)
        {
            current->size+=current->next->size + PADDED_SIZE(sizeof(struct block));
            current->next=current->next->next;
        }
        else
        {
            current=current->next;
        }
    }
}




void myfree(void *p)
{
  struct block *current = head;
  while(current!=NULL)
  {
    void *data_pointer = PTR_OFFSET(current, PADDED_SIZE(sizeof(struct block)));
    if (data_pointer == p)
    {
      current->in_use =0;
      coalesce(head);
      return;
    }
    current = current->next;
  }

}



void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main(){

void *p, *q, *r, *s;

p = myalloc(10); print_data();
q = myalloc(20); print_data();
r = myalloc(30); print_data();
s = myalloc(40); print_data();

myfree(q); print_data();
myfree(p); print_data();
myfree(s); print_data();
myfree(r); print_data();
}
