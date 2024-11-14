#include <stdio.h>
#include <stdlib.h>
#include "./string.h"

int nextline(string *dst) {
    size_t block_size = 8;
    size_t capacity = block_size;
    size_t length = 0;

    // Initialize the pointer
    *dst = (char *) calloc(capacity + 1, sizeof(char));
    if (*dst == NULL) {
        return -1;
    }

    // Read data into the string
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        // If the string is full, reallocate it
        if (length == capacity) {
            capacity += block_size;
            *dst = (char *) realloc(*dst, (capacity + 1) * sizeof(char));
            if (*dst == NULL) {
                free(*dst);
                return -1;
            }
        }

        // Add the current character to the string
        (*dst)[length++] = ch;
    }

    // NULL terminate the string
    (*dst)[length] = '\0';
    return 0;
}