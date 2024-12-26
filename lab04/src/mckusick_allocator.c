#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#define NUM_CLASSES 8

typedef struct Block {
    struct Block* next;
    size_t size;
} Block;

typedef struct Allocator {
    struct Block* freeLists[NUM_CLASSES];
    void* memoryStart;
    size_t totalSize;
} Allocator;

static size_t get_class(size_t size) {
    size_t classSize = 8;
    for (size_t i = 0; i < NUM_CLASSES; ++i) {
        if (size <= classSize) return i;
        classSize *= 2;
    }
    return NUM_CLASSES - 1;
}

Allocator* allocator_create(void *const memory, const size_t size) {
    if (!memory || size < NUM_CLASSES * sizeof(Block)) {
        const char* msg = "McKusick Allocator creation failed: invalid memory or size\n";
        write(1, msg, 59);
        return NULL;
    }

    Allocator* allocator = (Allocator*) memory;
    allocator->memoryStart = memory;
    allocator->totalSize = size;

    for (size_t i = 0; i < NUM_CLASSES; ++i) {
        allocator->freeLists[i] = NULL;
    }

    void* currentMemory = (void*)(allocator + 1);
    for (size_t i = 0; i < NUM_CLASSES; ++i) {
        size_t blockSize = 8 * (1 << i);
        while ((size_t)(currentMemory) + blockSize <= (size_t)(allocator->memoryStart) + allocator->totalSize) {
            Block* block = (Block*)currentMemory;
            block->size = blockSize;
            block->next = allocator->freeLists[i];
            allocator->freeLists[i] = block;

            currentMemory = (void*)((size_t)currentMemory + blockSize);
        }
    }

    const char* msg = "McKusick Allocator created successfully\n";
    write(1, msg, 40);

    return allocator;
}

void* allocator_alloc(Allocator *const allocator, const size_t size) {
    if (!allocator || size == 0) {
        const char* msg = "Allocation failed: invalid allocator or size\n";
        write(1, msg, 45);
        return NULL;
    }

    size_t classIndex = get_class(size);
    if (classIndex >= NUM_CLASSES) {
        const char* msg = "Allocation failed: size exceeds maximum class\n";
        write(1, msg, 47);
        return NULL;
    }

    Block* block = allocator->freeLists[classIndex];
    if (block) {
        allocator->freeLists[classIndex] = block->next;

        const char* msg = "Memory allocated successfully\n";
        write(1, msg, 30);
        
        return (void*)block;
    }

    size_t classSize = 8 * (1 << classIndex);

    Block* newBlock = (Block*)allocator->memoryStart;
    allocator->memoryStart = (void*)((size_t)allocator->memoryStart + classSize);

    newBlock->size = classSize;
    newBlock->next = NULL;

    const char* msg = "New block allocated\n";
    write(1, msg, 20);

    return (void*)newBlock;
}

void allocator_free(Allocator *const allocator, void *const memory) {
    if (!allocator || !memory) {
        const char* msg = "Free failed: invalid allocator or memory\n";
        write(1, msg, 41);
        return;
    }

    Block* block = (Block*)memory;
    size_t classIndex = get_class(block->size);
    block->next = allocator->freeLists[classIndex];
    allocator->freeLists[classIndex] = block;

    const char* msg = "Memory freed successfully\n";
    write(1, msg, 27);
}

void allocator_destroy(Allocator* allocator) {
    if (!allocator) {
        const char* msg = "McKusick Allocator destroy: NULL pointer\n";
        write(1, msg, 40);
        return;
    }

    const char* msg = "McKusick Allocator destroyed successfully\n";
    write(1, msg, 43);
}