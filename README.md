# Binary_Buddy_Memory_Management_System
Simper memory management system based on binary buddies

- int start_memory(int size) — initialzie the binary buddies memory system with "size" bytes.

- void *get_memory( int size ) — allocate a memory partition with “size” bytes for requested programs. It will return a
NULL pointer if there is no free space on the current memory block.

- void release_memory( void *p ) — release memory partition referenced by pointer “p” back to free space.

- void end_memory( void ) — print out memory leaks and release those leaks. 

LICENSE
Licensed under MIT: (http://opensource.org/licenses/MIT)
