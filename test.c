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
    
    int startMemory = 0;
    int memorySize = 0;
    int count = 0;
    char text[20];
    char allo[5] = "allo", quit[5] = "quit";

    printf("Enter the memory size:");
    scanf("%d",&startMemory);
    start_memory(startMemory);
    
    
    while(1){

        /* to handle user command. */
        fgets(text,sizeof text, stdin);
        strtok(text,"\n");
        if (strcmp(text, quit) == 0){
            
            if( count == 1){
                exit(0);
            }
            
            end_memory();
            exit(0);
        }
        else if(strcmp(text, allo) == 0 ){
            printf("Enter the amount you want to be allocated: ");
            scanf("%d",&memorySize);
            get_memory(memorySize);
        }
        else{
            printf("Enter the command. 'allo' to allocate memory; 'quit' to quit the system.\n\n");
        }
        count++;
    }
    
    
    // initialize memory size to 512.
    /*
    start_memory(512);
    printf("\n");

    test *t1 = get_memory(8);
    printf("Memory address for t1 is %p\n", (void *)t1);
    printf("--------------------------------\n");
    
    test *t2 = get_memory(8);
    //grow_memory(12, t2);
    printf("Memory address for t2 is %p\n", (void *)t2);
    printf("--------------------------------\n");
    
    
    test *t3 = get_memory(8);
    //pregrow_memory(12, t2);
    //test *t3 = get_memory((sizeof(test)));
    printf("Memory address for t3 is %p\n", (void *)t3);
    printf("--------------------------------\n");
    
    test *t4 = get_memory(8);
    //grow_memory(12, t4);
    printf("Memory address for t4 is %p\n", (void *)t4);
    printf("--------------------------------\n");
    
    //release_memory(t4);
    
    test *t5 = get_memory(8);
    //grow_memory(12, t2);
    printf("Memory address for t5 is %p\n", (void *)t5);
    printf("--------------------------------\n");
    
    test *t6 = get_memory(8);
    //grow_memory(12, t2);
    printf("Memory address for t6 is %p\n", (void *)t6);
    printf("--------------------------------\n");
    
    test *t7 = get_memory(8);
    //grow_memory(12, t2);
    printf("Memory address for t7 is %p\n", (void *)t7);
    printf("--------------------------------\n");
    
    test *t8 = get_memory(8);
    //grow_memory(12, t2);
    printf("Memory address for t8 is %p\n", (void *)t8);
    printf("--------------------------------\n");
    */
    
    /* to clean up all memory.*/
    //end_memory();
    
    return 1;
}