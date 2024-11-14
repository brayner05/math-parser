#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include "../util/string.h"
#include "Token.h"

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
    string source;
    size_t current;
    size_t start;
    Token *tokens;
    size_t token_count;
    size_t token_capacity;
    bool had_error;
    LexicalErrorList errors;
} Lexer;

/**
 * A constructor for the Lexer struct. Accepts a pointer to an
 * instance of a lexer, in which the default state of a lexer will
 * be written. That is `*lexer` will be a fresh lexer instance.
 * @param lexer the address of an empty lexer instance to write to
 */
void Lexer_Init(Lexer *lexer, string source);

Token *Lexer_Tokenize(Lexer *lexer);

#endif