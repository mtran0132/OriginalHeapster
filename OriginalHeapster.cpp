// OriginalHeapster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>


using namespace std;

struct Block {

	// # of bytes in the data section
	int block_size;

	// in C, you have to use "struct Block" as the type
	struct Block *next_block;	
};


// Global Constants
const int OVERHEAD_SIZE = sizeof(struct Block);
const int VOID_SIZE = sizeof(void*); 

// Will always point to the first free block in free list
struct Block *free_head;		

// PROTOTYPE DECLARATION
void my_initialize_heap(int size);
void* my_alloc(int size);
void my_free(void *data);

int main()
{
	my_initialize_heap(256);
	my_alloc(8);
}

void my_initialize_heap(int size) {
	free_head = (struct Block *) malloc(size);
	(*free_head).block_size = size - OVERHEAD_SIZE;
	(*free_head).next_block = NULL;
}


// Allocates a block of size "size"
void* my_alloc(int size) {
	//1 Round size up to VOID_SIZE
	//2 Searching for "temp": Block that fits rounded size.
	//3 WIth temp found, decide: split temp or not? and also, is temp == head or not?
	
	struct Block* temp = free_head;
	struct Block* newBlock;
	struct Block* previousBlock = temp; 
	int rounded_size; 

	if (size > VOID_SIZE) {
		rounded_size = VOID_SIZE * ceil((double)size / VOID_SIZE);
	}
	else {
		rounded_size = VOID_SIZE;
	}

	// Walk the list to find free block
	while (temp != NULL) {

		// If the block size is equal to the size or bigger
		if (temp->block_size >= rounded_size) {
			printf("Free Head Block Size:%i\n", free_head->block_size);

			// Break out to work with the block
			break;
		}

		// else the block is smaller than the rounded size, move on to next block
		else{
			temp = temp->next_block;
		}
		
	}

	// if block is head
	if (temp == free_head) {
		
		if (free_head->block_size >= rounded_size + OVERHEAD_SIZE) {

			newBlock = (struct Block*)(char*)temp + rounded_size + OVERHEAD_SIZE;
			newBlock->block_size = temp->block_size - (rounded_size + OVERHEAD_SIZE);
			newBlock->next_block = NULL;
			free_head = newBlock;
			return (struct Block*)((char*)temp + OVERHEAD_SIZE);

		}
		else {
			free_head = free_head->next_block;
			temp->next_block = NULL;
			return temp;
		}
	}
	// block is not head
	else {
		if (temp->block_size >= rounded_size + OVERHEAD_SIZE) {
			newBlock = (struct Block*)(char*)temp + rounded_size + OVERHEAD_SIZE;
			newBlock->block_size = temp->block_size - (rounded_size + OVERHEAD_SIZE);
			newBlock->next_block = temp->next_block;
		}
	}

	// Nothing was found.
	return 0;

}

// Frees the block
void my_free(void *data) {
	// shaded area move back to find the start of the block
	struct Block *temp = (struct Block*)((char *)data - OVERHEAD_SIZE);
	temp->next_block = free_head;
	free_head = temp;
}