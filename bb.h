/* bb.h header file for bb.c
 * Created by Zihao Wu.
 */

#ifndef _bb_h_
#define _bb_h_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct {
    int isUsed;
    int side;
    int size;
} Buddy;

void *get_memory(int size);
void *grow_memory(int size, void *p);
void *pregrow_memory(int size, void *p);
void release_memory(void *p);
int start_memory(int size);
void end_memory(void);

#endif 