/* =========================================================================================================
 * demo test program for our binary buddy memory management system.
 * =========================================================================================================
 * Copyright 2013-2015 Zihao Wu.
 * Licensed under MIT (https://github.com/zihaow/Binary_Buddy_Memory_Management_System/blob/master/LICENSE )
 * =========================================================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include "bb.h"

typedef struct {
    int mem;
} test;

int main(int argc, char **argv) {
    
    /*
    int startMemory = 0;
    int memorySize = 0;
    
    printf("Enter the memory size:");
    
    scanf("%d",&startMemory);
    start_memory(startMemory);
    
    while(1){
        printf("Enter the amount you want to be allocated:");
        scanf("%d",&memorySize);
        get_memory(memorySize);
    }
    */
    
    // initialize memory size to 512.
    start_memory(512);
    printf("\n");

    test *t1 = get_memory(4);
    printf("Memory address for t1 is %p\n", (void *)t1);
    printf("--------------------------------\n");
    
    test *t2 = get_memory(4);
    grow_memory(12, t2);
    printf("Memory address for t2 is %p\n", (void *)t2);
    printf("--------------------------------\n");
    
    test *t3 = get_memory(8);
    pregrow_memory(12, t2);
    printf("Memory address for t3 is %p\n", (void *)t3);
    printf("--------------------------------\n");
    
    test *t4 = get_memory(8);
    printf("Memory address for t4 is %p\n", (void *)t4);
    printf("--------------------------------\n");
    
    /* to clean up all memory.*/
    end_memory();
    
    return 1;
}
