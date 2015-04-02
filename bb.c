/* =========================================================================================================
 * bb.c
 * =========================================================================================================
 * bb stands for 'binary buddies'. Simper memory management system based on binary buddies.
 * Copyright 2013-2015 Zihao Wu.
 * Licensed under MIT (https://github.com/zihaow/Binary_Buddy_Memory_Management_System/blob/master/LICENSE )
 * =========================================================================================================
 */

#include "bb.h"

#define IS_USED_FALSE 0
#define IS_USED_TRUE 1
#define SIDE_IS_LEFT 0
#define SIDE_IS_RIGHT 1
#define IS_POWER_OF_2(x) (!((x)&((x)-1)))

int freeSpace = 0;
int depth = 0;
int startMemory = 0;
void *bbp;
int data[10] = {0.0};
int counter = 0;
int deepestBlockSize = 0;
int preDeepestBlockSize = 0;

/* Initializes the memory module and the program can only access "size" bytes of memory. */
int start_memory(int size) {
    if ((bbp = malloc(size))) {
        
        startMemory = size;
        freeSpace = size;
        printf("address is %p\n", bbp);
        
        
        Buddy *bb = bbp;
        bb -> size = size;
        bb -> isUsed = IS_USED_FALSE;
        bb -> side = SIDE_IS_LEFT;
        return 1;
    }
    else {
        return 0;
    }
}

/* split_node function to split one big node to smaller pieces 
 so that it will fit in for smaller memory patitions. */
void split_node(Buddy *bb){
    
    bb -> side = SIDE_IS_LEFT;
    bb -> isUsed = IS_USED_FALSE;
    
    Buddy *new;
    new = (void *)bb + (bb -> size)/2;

    new -> isUsed = IS_USED_FALSE;
    new -> side = SIDE_IS_RIGHT;
    new -> size = (bb -> size)/2;
    
    bb -> size = (bb -> size)/2;
    
    
    printf("current address is %p\n", new);
    printf("total size is %d\n", (new->size));
    printf("Side is %d\n", (new->side));
    printf("Is used is %d\n", (new->isUsed));
}

/* best_fit function to find the best fit position for allocating the memory. */
int Best_fit(int partition){
    int position = 0;
    int result = 0;
    
    /* temp pointer points to our memory patition. */
    void *temp = NULL;
    
    //printf("size **** is %d\n", partition);
    
    /* find the best position for partition to be allocated. */
    while(position < startMemory){
        temp = bbp + position*deepestBlockSize;
        if( ((Buddy *)(void *)temp) -> isUsed == IS_USED_FALSE && (((Buddy *)(void *)temp) -> size >= partition)  ){
            //printf("size **** is %d\n", (((Buddy *)(void *)temp) -> size));
            
            //printf("addrees before is %p\n", temp);
            //addrees before is 0x7ff140404c10
            result = position*deepestBlockSize;
            break;
            
        }
        position++;
    }
    return result;
}

/* allocate a memory partition with "size" bytes in it. */
void *get_memory(int size) {
    
    /* do nothing if requested size is zero. */
    if(size == 0){
        return NULL;
    }
    
    int partitionSize = 2;
    int count = 0;
    size = size + sizeof(Buddy);
    void *temp = NULL;
    
    /* check how many times "get_memory" is called. */
    counter++;
    
    /* case when size is a power of 2. Then the block size is exactly the value of "size". */
    if (IS_POWER_OF_2(size)){
        while(partitionSize != size){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
        deepestBlockSize = size;
    }
    
    /* case when size is not a power of 2. Then the block size is the closest number to the power of 2. */
    if (!IS_POWER_OF_2(size)){
        while(partitionSize <= size){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
        deepestBlockSize = size;
    }
    
    //printf("partition size is %d\n", partitionSize);
    
    /* case when we don't have enough space for memory allication. */
    if(freeSpace < partitionSize*2){
        printf("free space is %d\n", freeSpace);
        printf("Not enough space to be allocated, you need an extra %d to allocate the memory. \n", partitionSize*2);
        return NULL;
    }
    
    /* make sure we always have recorded the deepest block size. */
    if(counter == 1){
        preDeepestBlockSize = deepestBlockSize;
    }
    if(deepestBlockSize > preDeepestBlockSize){
        deepestBlockSize = preDeepestBlockSize;
    }
    
    /* bb pointer to our struct defined in bb.h. */
    Buddy *bb = bbp;
    
    /* for sure we want to split nodes at the very beginning. */
    if(counter == 1){
        while( (bb -> size) != partitionSize ){
            split_node(bb);
        }
    }
    
    /* find the best location for memory allocation. */
    int location = 0;
    location = Best_fit(partitionSize);
    temp = bbp + location;
    
    /* print the current block size. */
    //int result = 0;
    //result = (((Buddy *)temp) -> size);
    //printf("result is %d\n", result);
    
    /* case when we need to split a node since partition size is much smaller. */
    if( ((Buddy *)temp) -> size != partitionSize ){
        int split_times = 0;
        split_times = (((Buddy *)temp) -> size)/partitionSize;
        
        for(int i =0; i< split_times/2;i++){
            split_node(temp);
            printf("Spliting nodes. \n");
            if( ((Buddy *)temp) -> size == partitionSize ){
                break;
            }
        }
    }
    
    /* assgin properties to our assigned memory block. */
    ((Buddy *)temp) -> isUsed = IS_USED_TRUE;
    ((Buddy *)temp) -> size = partitionSize;
    
    /* assign memory position to be returned. */
    bb = temp;
    
    /* store partition size data to an array for future reference. */
    data[counter] = partitionSize;
    
    /* calculate the free space left to be allocated. */
    freeSpace = freeSpace - partitionSize;
    printf("free space is %d\n", freeSpace);
    
    return (void *)bb;
}


/* [release_memory] to release memory. */
void release_memory(void *p) {
    int bbSize = 0;
    bbSize = (((Buddy *)p) -> size);
    void *temp;
    
    
    if( ((Buddy *)p) -> side == SIDE_IS_LEFT){
        
        temp = p + bbSize;
        //printf("addrees before is %p\n", temp);
        if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE){
            ((Buddy *)(void *)p) -> side = SIDE_IS_LEFT;
            ((Buddy *)(void *)p) -> size = bbSize*2;
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize*2;
            
        }
        else{
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize;
            
        }
    }
    
    if( ((Buddy *)p) -> side == SIDE_IS_RIGHT){
        temp = p - bbSize;
        //printf("addrees before is %p\n", temp);
        if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE){
            p = temp;
            ((Buddy *)(void *)p) -> side = SIDE_IS_LEFT;
            ((Buddy *)(void *)p) -> size = bbSize*2;
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize*2;
        }
        else{
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize;
        }
    }
    //printf("free space after memory release is %d\n", freeSpace);
    printf("Released memory at %p\n", p);
}

/* end_memory to end memory and print out potential memory leak and then releases them. */
void end_memory(void) {
    void *temp = NULL;
    
    for (int i=0; i< startMemory; i=i+deepestBlockSize){
        temp = bbp + i;
        if (((Buddy *)temp)->isUsed == IS_USED_TRUE){
            printf("***** Memory leak at ***** %p\n", temp);
            printf("\n");
            
            /* call release_memory(void *p) to release memory. */
            release_memory(temp);
        }
    }
}
