#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Store in dst a substring of src from start (inclusive) to end (exclusive)
#define substring(dst, src, start, end) memcpy((dst), &(src)[(start)], (end) - (start))

/**
 * Expression Parser for the grammar:
 * ====================================
 * S -> Expr
 * Expr -> Term ((+ | -) Term)*
 * Term -> Factor ((* | /) Factor)*
 * Factor -> [0-9]+(.[0-9]*)? | (Expr)
 */

// `char*` redefined as string for readability
typedef char* string;

// Types of tokens to accept
typedef enum {
    TK_PLUS, TK_MINUS, TK_STAR, TK_SLASH,
    TK_LPAREN, TK_RPAREN,
    TK_NUMBER
} TokenType;

// Token structure
typedef struct {
    TokenType type;
} Token;

// A structure representing a lexical error
typedef struct LexicalError {
    struct LexicalError *next;
    string message;
} LexicalError;

// A linked list of lexer errors
typedef struct {
    LexicalError *first;
    LexicalError *last;
} LexicalErrorList;

// The lexer/tokenizer
typedef struct {
    Token *tokens;
    size_t current;
    size_t start;
    LexicalErrorList errors;
} Lexer;

/**
 * A constructor for the Lexer struct. Accepts a pointer to an
 * instance of a lexer, in which the default state of a lexer will
 * be written. That is `*lexer` will be a fresh lexer instance.
 * @param lexer the address of an empty lexer instance to write to
 */
void Lexer_Init(Lexer *lexer) {
    lexer->tokens = NULL;
    lexer->current = 0;
    lexer->start = 0;
    lexer->errors = (LexicalErrorList) { .first = NULL, .last = NULL };
}

/**
 * Add a lexical error to the lexer without crashing the application.
 * This is done so that the user can get as much information as possible
 * about what went wrong, rather than terminating after a single error.
 * @param lexer the lexer to operate on
 * @param message the error message
 */
void Lexer_AddError(Lexer *lexer, string message) {
    LexicalError *error = (LexicalError *) malloc(sizeof(LexicalError));
    *error = (LexicalError) { 
        .next = NULL,
        .message = message
    };

    if (lexer->errors.first == NULL) {
        lexer->errors.first = lexer->errors.last = error;
        return;
    }

    lexer->errors.last->next = error;
    lexer->errors.last = error;
}

/**
 * Read a single line from the user and store that line in the
 * string `*dst`.
 * @param dst a pointer to a string to which the line will be written
 */
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
}

int main(void) {
    while (1) {
        printf("expr > ");
        string line;
        nextline(&line);
        if (strcmp(line, "quit") == 0) {
            free(line);
            exit(0);
        }

        Lexer lexer;
        Lexer_Init(&lexer);

        free(line);
    }
}