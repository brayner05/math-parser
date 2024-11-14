#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define substring(dst, src, start, end) memcpy((dst), &(src)[(start)], (end) - (start))

/**
 * Expression Parser for the grammar:
 * ====================================
 * S -> Expr
 * Expr -> Term ((+ | -) Term)*
 * Term -> Factor ((* | /) Factor)*
 * Factor -> [0-9]+(.[0-9]*)? | (Expr)
 */

typedef char* string;

// Types of tokens to accept
typedef enum {
    TK_PLUS, TK_MINUS, TK_STAR, TK_SLASH,
    TK_LPAREN, TK_RPAREN,
    TK_NUMBER
} TokenType;

typedef struct {
    TokenType type;
} Token;

typedef struct LexicalError {
    struct LexicalError *next;
    string message;
} LexicalError;

typedef struct {
    LexicalError *first;
    LexicalError *last;
} LexicalErrorList;

typedef struct {
    Token *tokens;
    size_t current;
    size_t start;
    LexicalErrorList errors;
} Lexer;

void Lexer_Init(Lexer *lexer) {
    lexer->tokens = NULL;
    lexer->current = 0;
    lexer->start = 0;
    lexer->errors = (LexicalErrorList) { .first = NULL, .last = NULL };
}

void Lexer_AddError(Lexer *lexer, string message) {
    if (lexer->errors.first == NULL) {
        // LexicalError
        // lexer->errors.first = lexer->errors.last = 
    }
}

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
        free(line);
    }
}