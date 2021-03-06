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
void menu();
void testOne();
void testTwo();
void testThree();
void testFour();
void testFive();

int main()
{

	int choice = 1;
	while (choice != 0) {
		menu();
		cin >> choice;

		switch (choice) {
		case(1) : testOne();	break;
		case(2) : testTwo();	break;
		case(3) : testThree();  break;
		case(4) : testFour();	break;
		case(5) : testFive();	break;
		}
	}

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

			// Break out to work with the block
			break;
		}

		// else the block is smaller than the rounded size, move on to next block
		else{
			
			previousBlock = temp;
			temp = temp->next_block;
		}	
	}

	// if block is head
	if (temp == free_head) {
		
		// Split and Head
		if (temp->block_size >= rounded_size + OVERHEAD_SIZE + VOID_SIZE) {

			// pointer addition using char
			struct Block* newBlock = (struct Block*)(char*)temp + rounded_size + OVERHEAD_SIZE;
			newBlock->block_size = temp->block_size - (size + OVERHEAD_SIZE);
			
			temp->block_size = rounded_size;
			// Get temp's block chain and assign it to newBlock's chain
			newBlock->next_block = temp->next_block;

			// newBlock should be free, therefore assign
			free_head = newBlock;

			// returns the address to the data portion of the block
			return (struct Block*)((char*)temp + OVERHEAD_SIZE);

		}

		// Head and not split
		else {
			free_head = temp->next_block;
			temp->next_block = NULL;
			return (struct Block*)((char*)temp + OVERHEAD_SIZE);
		}
	}
	// block is not head
	else {

		// Split and not head
		if (temp->block_size >= rounded_size + OVERHEAD_SIZE + VOID_SIZE) {

			// pointer addition using char
			struct Block* newBlock = (struct Block*)(char*)temp + rounded_size + OVERHEAD_SIZE;
			newBlock->block_size = temp->block_size - (size + OVERHEAD_SIZE);
			temp->block_size = rounded_size;

			// Link up the previous block with 
			previousBlock->next_block = newBlock;

			// Get temp's block chain and assign it to newBlock's chain
			newBlock->next_block = temp->next_block;

			// returns the address to the data portion of the block
			return (struct Block*)((char*)temp + OVERHEAD_SIZE);

		}

		// Not head, not split
		else {

			// Link up previous block to temp's next block
			previousBlock->next_block = temp->next_block;
			return (struct Block*)((char*)temp +OVERHEAD_SIZE);
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


void menu() {
	cout << "Test Choice [1-5]: " << "\n";
	cout << "'0' to exit\n";

}

void testOne() {
	my_initialize_heap(512);
	cout << "/-------Test 1 -------/" << "\n";
	cout << "Allocated one int then freed it" << "\n" << "Allocated another int then freed it" << "\n";
	cout << "Addresses should be the same!\n";
	int *x = (int*)my_alloc(sizeof(int));
	printf("Address of X: %\t\n", x);
	my_free(x);
	printf("X has been freed.\n");

	int *y = (int*)my_alloc(sizeof(int));
	printf("Address of Y: %p\t\n", y);
	my_free(y);
	printf("Y has been freed.\n");
}

void testTwo() {
	my_initialize_heap(512);
	cout << "/-------Test 2 -------/" << "\n";
	cout << "Allocated two ints\n" << "\n";
	int *x = (int*)my_alloc(sizeof(int));
	printf("Address of X: %p\t\n", x);
	int *y = (int*)my_alloc(sizeof(int));
	printf("Address of Y: %p\t\n", y);

	my_free(x);
	my_free(y);
}

void testThree() {
	my_initialize_heap(512);
	cout << "/-------Test 3 -------/" << "\n";
	
	printf("Allocating an int, X\n");
	int *x = (int*)my_alloc(sizeof(int));
	printf("Address of X: %p\t\n", x);
	
	printf("Allocating an int, Y\n");
	int *y = (int*)my_alloc(sizeof(int));
	printf("Address of Y: %p\t\n", y);

	printf("Allocating an int, Z\n");
	int *z = (int*)my_alloc(sizeof(int));
	printf("Address of Z: %p\t\n", z);

	my_free(y);
	printf("Y has been freed.\n");

	double *d = (double*)my_alloc(sizeof(double) * 2);
	printf("Double Address: %p\t\n", d);

	printf("Allocating an int, A\n");
	int *a = (int*)my_alloc(sizeof(int));
	printf("Address of A: %p\t\n", a);
	printf("Should be the same as Address of Y\n");

}

void testFour() {
	my_initialize_heap(512);
	cout << "/-------Test 4 -------/" << "\n";
	printf("Allocated a char, c\n");
	char *c = (char*)my_alloc(sizeof(char));

	printf("Allocated an int, x\n");
	int *x = (int*)my_alloc(sizeof(int));
	printf("Address of Char: %p\t\n", c);
	printf("Address of int: %p\t\n", x);
}

void testFive() {
	my_initialize_heap(512);
	cout << "/-------Test 5 -------/" << "\n";
	printf("Allocate an 80 int array then allocate one more int, then free the array\n");
	int* arr = (int*)my_alloc((80 * sizeof(int)));
	int* i = (int*)my_alloc(sizeof(int));
	printf("Array Address: %p\t\nint Address: %p\n", arr, i);
	my_free(arr);
	printf("Arr has been freed.\n");
	printf("int address: %p\n", i);
}