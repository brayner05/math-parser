#include <stdio.h>
#include <stdlib.h>
#include "util/string.h"
#include "lexer/Lexer.h"

/**
 * Expression Parser for the grammar:
 * ====================================
 * S -> Expr
 * Expr -> Term ((+ | -) Term)*
 * Term -> Factor ((* | /) Factor)*
 * Factor -> [0-9]+(.[0-9]*)? | (Expr)
 */

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
    return 0;
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
        Lexer_Init(&lexer, line);
        Token *tokens = Lexer_Tokenize(&lexer);

        for (int i = 0; i < lexer.token_count; i++) {
            Token *token = &tokens[i];
            printf("Type: %s, Lexeme: \"%s\", Value: %lf\n", TokenType_ToString(token->type), token->lexeme, token->value);
        }

        if (lexer.had_error) {
            for (LexicalError *err = lexer.errors.first; err != NULL; err = err->next) {
                puts(err->message);
            }
        }
    
        free(line);
    }
}