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
    
    /* find the best position for partition to be allocated. */
    while(position < startMemory){
        
        temp = bbp + position*deepestBlockSize;
        if( ((Buddy *)(void *)temp) -> isUsed == IS_USED_FALSE && (((Buddy *)(void *)temp) -> size >= partition)  ){
            
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
        deepestBlockSize = partitionSize;
    }
    
    /* cases when size is not a power of 2. Then the block size is the closest number to the power of 2. */
    if (!IS_POWER_OF_2(size)){
        while(partitionSize <= size){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
        deepestBlockSize = partitionSize;
    }
    
    /* cases when we don't have enough space for memory allication. */
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
    
    /* cases when we need to split a node since partition size is much smaller. */
    if( ((Buddy *)temp) -> size != partitionSize ){
        int split_times = 0;
        split_times = (((Buddy *)temp) -> size)/partitionSize;
        int i =0;
        
        for(i =0; i< split_times/2;i++){
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

void *grow_memory(int size, void *p){
    
    int growSize = 0;
    int partitionSize = 2;
    int prePartitionSize = 2;
    int count = 0;
    void *temp =  NULL;
    void *temp2 = NULL;
    void *result = NULL;
    
    
    prePartitionSize = (((Buddy *)p) -> size);
    growSize = size + (((Buddy *)p) -> size);
    
    /* cases when growsize is a power of 2. */
    if (IS_POWER_OF_2(growSize)){
        while(partitionSize != growSize){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
    }
    
    /* cases when growsize is not a power of 2.
     Then the block size is the closest number to the power of 2. */
    if (!IS_POWER_OF_2(growSize)){
        while(partitionSize <= growSize){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
    }
    
    int check_times = 0;
    int can_grow = 0;
    check_times = (partitionSize*2)/prePartitionSize;
    
    /* cases when buddy to grow is on the left side. */
    if( (((Buddy *)p) -> side) == SIDE_IS_LEFT){
        int i = 0;
        for(i = 1; i< check_times; i++){
            temp = p+i * prePartitionSize;
            
            if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE ){
                can_grow++;
            }
        }
        
        /* this means that we have enoug space to grow on p's current address. */
        if( can_grow == (check_times-1)){
        
            /* set the properties for "((Buddy *)p)". */
            (((Buddy *)p) -> size) = partitionSize;
            (((Buddy *)p) -> side) = SIDE_IS_LEFT;
            (((Buddy *)p) -> isUsed) = IS_USED_TRUE;
        
            /* set the properties for "((Buddy *)p)"'s buddy, which is the
             one on its right. */
            temp = p + partitionSize;
            ((Buddy *)temp) -> side = SIDE_IS_RIGHT;
            ((Buddy *)temp) -> size = partitionSize;
            ((Buddy *)temp) -> isUsed = IS_USED_FALSE;
            
            result = p;
            freeSpace = freeSpace - prePartitionSize;
        }
        else{
            
            /* create a new buddy. */
            Buddy *new = p;
            
            int position = 0;
            position = Best_fit(partitionSize);
            
            if(position == 0){
                printf("No enough space to grow. \n");
            }
            else{
                /* set the properties for "((Buddy *)p)". */
                ((Buddy *)new) -> side = SIDE_IS_LEFT;
                ((Buddy *)new) -> size = partitionSize;
                new = (void *)p + position;
                ((Buddy *)new) -> isUsed = IS_USED_TRUE;
                
                /* set the properties for "((Buddy *)p)"'s buddy, which is the
                 one on its right. */
                temp2 = (void *)p + position + partitionSize;
                ((Buddy *)temp2) -> side = SIDE_IS_RIGHT;
                ((Buddy *)temp2) -> size = partitionSize;
                ((Buddy *)temp2) -> isUsed = IS_USED_FALSE;
                
                result = new;
                freeSpace = freeSpace - prePartitionSize;
            }
        }
    }
    
    /* cases when buddy to grow is on the right side. */
    if( ((Buddy *)p) -> side == SIDE_IS_RIGHT){
        int i = 0;
        for(i = 1; i < check_times; i++){
            temp = (void *)p-i * prePartitionSize;
            if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE ){
                can_grow++;
            }
            temp2 = (void *)p+i * prePartitionSize;
            if( ((Buddy *)temp2) -> isUsed == IS_USED_FALSE ){
                can_grow++;
            }
        }
        
        if( can_grow == (check_times-2)){
            
            /* set the properties for "((Buddy *)p)". */
            (((Buddy *)(void *)p) -> size) = partitionSize;
            (((Buddy *)(void *)p) -> side) = SIDE_IS_RIGHT;
            (((Buddy *)(void *)p) -> isUsed) = IS_USED_TRUE;
            
            /* set the properties for "((Buddy *)p)"'s buddy, which is the
             one on its right. */
            temp2 = (void *)p - partitionSize;
            ((Buddy *)temp2) -> side = SIDE_IS_LEFT;
            ((Buddy *)temp2) -> size = partitionSize;
            ((Buddy *)temp2) -> isUsed = IS_USED_FALSE;
            
            result = p;
            freeSpace = freeSpace - prePartitionSize;
        }
        else{
            /* create a new buddy. */
            Buddy *new = p;
            
            int position = 0;
            position = Best_fit(partitionSize);
            
            if(position == 0){
                printf("No enough space to grow. \n");
            }
            else{
                
                /* set the properties for "((Buddy *)p)". */
                ((Buddy *)new) -> side = SIDE_IS_LEFT;
                ((Buddy *)new) -> size = partitionSize;
                new = (void *)p + position;
                ((Buddy *)new) -> isUsed = IS_USED_TRUE;
                
                /* release the previous memory. */
                release_memory(p);
                
                /* set the properties for "((Buddy *)p)"'s buddy, which is the
                 one on its right. */
                temp2 = (void *)p + position + partitionSize;
                ((Buddy *)temp2) -> side = SIDE_IS_RIGHT;
                ((Buddy *)temp2) -> size = partitionSize;
                ((Buddy *)temp2) -> isUsed = IS_USED_FALSE;
                
                result = new;
                freeSpace = freeSpace - prePartitionSize;
            }
        }
    }
    return result;
}

void *pregrow_memory(int size, void *p){
    
    int growSize = 0;
    int partitionSize = 2;
    int prePartitionSize = 2;
    int count = 0;
    void *temp =  NULL;
    void *temp2 = NULL;
    void *result = NULL;
    void *from = p;
    
    
    prePartitionSize = (((Buddy *)p) -> size);
    growSize = size + (((Buddy *)p) -> size);
    
    /* cases when growsize is a power of 2. */
    if (IS_POWER_OF_2(growSize)){
        while(partitionSize != growSize){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
    }
    
    /* cases when growsize is not a power of 2.
     Then the block size is the closest number to the power of 2. */
    if (!IS_POWER_OF_2(growSize)){
        while(partitionSize <= growSize){
            
            partitionSize = (int)pow(2.0, ((double)count));
            count++;
        }
    }
    
    int check_times = 0;
    int can_grow = 0;
    check_times = (partitionSize-prePartitionSize)/prePartitionSize;
    
    /* cases when buddy to grow is on the left side. */
    if( (((Buddy *)p) -> side) == SIDE_IS_LEFT){
        int i = 0;
        
        /* check on parts below the Buddy *p. */
        for(i = 1; i< (check_times+1); i++){
            temp = (void *)p+i * prePartitionSize;
            if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE){
                can_grow++;
            }
        }
        
        /* check on parts at the top of Buddy *p. */
        for(i = 1; i< check_times; i++){
            temp2 = (void *)p-i * prePartitionSize;
            if( ((Buddy *)temp2) -> isUsed == IS_USED_FALSE ){
                can_grow++;
            }
        }
        
        /* this means that we have enoug space to grow on p's current address. */
        if( can_grow == (check_times*2) && can_grow != 0 ){
            
            p = (void *)p - (prePartitionSize * check_times);
            
            /* set the properties for "((Buddy *)p)". */
            (((Buddy *)p) -> size) = partitionSize;
            (((Buddy *)p) -> side) = SIDE_IS_LEFT;
            (((Buddy *)p) -> isUsed) = IS_USED_TRUE;
            
            /* set the properties for "((Buddy *)p)"'s buddy, which is the
             one on its right. */
            temp = (void *)from + prePartitionSize;
            ((Buddy *)temp) -> side = SIDE_IS_RIGHT;
            ((Buddy *)temp) -> size = partitionSize;
            ((Buddy *)temp) -> isUsed = IS_USED_FALSE;
            
            result = p;
            freeSpace = freeSpace - prePartitionSize;
        }
        else{
            
            /* create a new buddy. */
            Buddy *new = p;
            
            int position = 0;
            position = Best_fit(partitionSize);
            
            if(position == 0){
                printf("No enough space to grow. \n");
            }
            else{
                /* set the properties for "((Buddy *)p)". */
                ((Buddy *)new) -> side = SIDE_IS_LEFT;
                ((Buddy *)new) -> size = partitionSize;
                new = (void *)p + position;
                ((Buddy *)new) -> isUsed = IS_USED_TRUE;
                
                /* set the properties for "((Buddy *)p)"'s buddy, which is the
                 one on its right. */
                temp2 = (void *)p + position + partitionSize;
                ((Buddy *)temp2) -> side = SIDE_IS_RIGHT;
                ((Buddy *)temp2) -> size = partitionSize;
                ((Buddy *)temp2) -> isUsed = IS_USED_FALSE;
                
                result = new;
                freeSpace = freeSpace - partitionSize;
            }
        }
    }
    
    /* cases when buddy to grow is on the right side. */
    if( (((Buddy *)p) -> side) == SIDE_IS_RIGHT){
        
        check_times = 2*check_times + 1;
        int i = 0;
        for(i = 1; i< check_times; i++){
            temp = (void *)p-i * prePartitionSize;
            if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE ){
                can_grow++;
            }
        }
        
        if( can_grow == check_times && can_grow != 0 ){
            
            p = (void *)p - ((check_times-1)/2) * prePartitionSize;
            
            /* set the properties for "((Buddy *)p)". */
            (((Buddy *)(void *)p) -> size) = partitionSize;
            (((Buddy *)(void *)p) -> side) = SIDE_IS_RIGHT;
            (((Buddy *)(void *)p) -> isUsed) = IS_USED_TRUE;
            
            /* set the properties for "((Buddy *)p)"'s buddy, which is the
             one on its right. */
            temp2 = (void *)p - check_times * prePartitionSize;
            ((Buddy *)temp2) -> side = SIDE_IS_LEFT;
            ((Buddy *)temp2) -> size = partitionSize;
            ((Buddy *)temp2) -> isUsed = IS_USED_FALSE;
            
            result = p;
            freeSpace = freeSpace - prePartitionSize;
        }
        else{
            /* create a new buddy. */
            Buddy *new = p;
            
            int position = 0;
            position = Best_fit(partitionSize);
            
            if(position == 0){
                printf("No enough space to grow. \n");
            }
            else{
                
                /* set the properties for "((Buddy *)p)". */
                ((Buddy *)new) -> side = SIDE_IS_LEFT;
                ((Buddy *)new) -> size = partitionSize;
                new = (void *)p + position;
                ((Buddy *)new) -> isUsed = IS_USED_TRUE;
                
                /* release the previous memory. */
                release_memory(p);
                
                /* set the properties for "((Buddy *)p)"'s buddy, which is the
                 one on its right. */
                temp2 = (void *)p + position + partitionSize;
                ((Buddy *)temp2) -> side = SIDE_IS_RIGHT;
                ((Buddy *)temp2) -> size = partitionSize;
                ((Buddy *)temp2) -> isUsed = IS_USED_FALSE;
                
                result = new;
                freeSpace = freeSpace - prePartitionSize;
            }
        }
    }
    return result;
}


/* [release_memory] to release memory. */
void release_memory(void *p) {
    int bbSize = 0;
    bbSize = (((Buddy *)p) -> size);
    void *temp;
    
    
    if( ((Buddy *)p) -> side == SIDE_IS_LEFT){
        
        temp = p + bbSize;
        
        if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE){
            ((Buddy *)(void *)p) -> side = SIDE_IS_LEFT;
            ((Buddy *)(void *)p) -> size = bbSize*2;
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize;
        }
        else{
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize;
        }
    }
    
    if( ((Buddy *)p) -> side == SIDE_IS_RIGHT){
        temp = p - bbSize;
        
        if( ((Buddy *)temp) -> isUsed == IS_USED_FALSE){
            p = temp;
            ((Buddy *)(void *)p) -> side = SIDE_IS_LEFT;
            ((Buddy *)(void *)p) -> size = bbSize*2;
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize;
        }
        else{
            ((Buddy *)(void *)p) -> isUsed = IS_USED_FALSE;
            freeSpace = freeSpace + bbSize;
        }
    }
    printf("free space after memory release is %d\n", freeSpace);
    printf("Released memory at %p\n", p);
}

/* end_memory to end memory and print out potential memory leak and then releases them. */
void end_memory(void) {
    void *temp = NULL;
    int i = 0;
    for (i = 0; i< startMemory; i=i+deepestBlockSize){
        temp = bbp + i;
        if (((Buddy *)temp)->isUsed == IS_USED_TRUE){
            printf("***** Memory leak at ***** %p\n", temp);
            printf("\n");
            
            /* call release_memory(void *p) to release memory. */
            release_memory(temp);
        }
    }
}