#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

typedef struct Allocator {
    struct Block* freeList;
    void* memoryStart;
    size_t totalSize;
} Allocator;

Allocator* allocator_create(void *const memory, const size_t size) {
    if (!memory || size < sizeof(struct Block)) return NULL;

    Allocator* allocator = (Allocator*) memory;
    allocator->memoryStart = memory;
    allocator->totalSize = size;

    allocator->freeList = (struct Block*)((char*)memory + sizeof(Allocator));
    allocator->freeList->size = size - sizeof(Allocator);
    allocator->freeList->next = NULL;

    const char* msg = "FreeList Allocator created successfully\n";
    write(1, msg, 40);

    return allocator;
}

void* allocator_alloc(Allocator *const allocator, const size_t size) {
    if (!allocator || size == 0) {
        const char* msg = "Memory allocation error: invalid allocator or size.\n";
        write(1, msg, 55);
        return NULL;
    }

    struct Block** current = &allocator->freeList;
    while (*current) {
        if ((*current)->size >= size) {
            if ((*current)->size > size + sizeof(struct Block)) {
                struct Block* remaining = (struct Block*)((char*)(*current) + sizeof(struct Block) + size);
                remaining->size = (*current)->size - size - sizeof(struct Block);
                remaining->next = (*current)->next;

                (*current)->size = size;
                *current = remaining;
            } else {
                *current = (*current)->next;
            }

            const char* allocMsg = "Memory allocated successfully\n";
            write(1, allocMsg, 30);

            return (void*)((char*)(*current) + sizeof(struct Block));
        }
        current = &(*current)->next;
    }

    const char* errorMsg = "No suitable block found\nMemory allocation error.\n";
    write(1, errorMsg, 46);
    return NULL;
}

void allocator_free(Allocator *const allocator, void *const memory) {
    if (!allocator || !memory) return;

    struct Block* block = (struct Block*)((char*)memory - sizeof(struct Block));
    block->next = allocator->freeList;
    allocator->freeList = block;

    const char* msg = "Memory freed successfully\n";
    write(1, msg, 27);
}

void allocator_destroy(Allocator* allocator) {
    if (!allocator) {
        const char* msg = "FreeList Allocator destroy: NULL pointer\n";
        write(1, msg, 39);
        return;
    }
    const char* msg = "FreeList Allocator destroyed successfully\n";
    write(1, msg, 42);
}