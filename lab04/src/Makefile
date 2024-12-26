CC = gcc
CFLAGS = -Wall -Werror -g -fPIC
LDFLAGS = -ldl

MAIN_SRC = main.c
ALLOCATOR_SRC_1 = freelist_allocator.c
ALLOCATOR_SRC_2 = mckusick_allocator.c

MAIN_OBJ = main.o
ALLOCATOR_OBJ_1 = freelist_allocator.o
ALLOCATOR_OBJ_2 = mckusick_allocator.o

ALLOCATOR_LIB_1 = freelist_allocator.so
ALLOCATOR_LIB_2 = mckusick_allocator.so

TARGET = main

all: $(TARGET) $(ALLOCATOR_LIB_1) $(ALLOCATOR_LIB_2)

$(TARGET): $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) -o $(TARGET) $(LDFLAGS)

$(ALLOCATOR_LIB_1): $(ALLOCATOR_OBJ_1)
	$(CC) -shared $(ALLOCATOR_OBJ_1) -o $(ALLOCATOR_LIB_1)

$(ALLOCATOR_LIB_2): $(ALLOCATOR_OBJ_2)
	$(CC) -shared $(ALLOCATOR_OBJ_2) -o $(ALLOCATOR_LIB_2)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN_OBJ) $(ALLOCATOR_OBJ_1) $(ALLOCATOR_OBJ_2) $(TARGET) $(ALLOCATOR_LIB_1) $(ALLOCATOR_LIB_2)