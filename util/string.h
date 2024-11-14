#ifndef STRING_H
#define STRING_H

#include <string.h>

typedef char *string;

// Store in dst a substring of src from start (inclusive) to end (exclusive)
#define substring(dst, src, start, end) strncpy((dst), &(src)[(start)], (end) - (start))

#endif