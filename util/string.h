#ifndef STRING_H
#define STRING_H

#include <string.h>

typedef char *string;

// Store in dst a substring of src from start (inclusive) to end (exclusive)
#define substring(dst, src, start, end) strncpy((dst), &(src)[(start)], (end) - (start))

/**
 * Read a single line from the user and store that line in the
 * string `*dst`.
 * @param dst a pointer to a string to which the line will be written
 */
int nextline(string *dst);

#endif