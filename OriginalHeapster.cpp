// OriginalHeapster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

using namespace std;

struct Block {
	int block_size;				// # of bytes in the data section
	struct Block *next_block;	// in C, you have to use "struct Block" as the type
};

const int OVERHEAD_SIZE = sizeof(struct Block);
const int VOID_SIZE = sizeof(void*);

struct Block *free_head;		// Will always point to the first free block in free list

int main()
{
    cout << "Hello World!\n"; 
	cout << VOID_SIZE;
	cout << OVERHEAD_SIZE;
}

void my_initialize_heap(int size) {
	free_head = (struct Block *) malloc(size);

	// Ask Neal for help here
	// Free space = 
	(*free_head).block_size = size - OVERHEAD_SIZE;
	(*free_head).next_block = NULL;
}


// Allocates a block of size "size"
void my_alloc(int size) {
	//1 Round size up to VOID_SIZE
	//2 Searching for "temp": Block that fits rounded size.
	//3 WIth temp found, decide: split temp or not? and also, is temp == head or not?
}


// Frees the block
void my_free(void *data) {
	// shaded area move back to find the start of the block
}