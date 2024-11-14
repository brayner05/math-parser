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

/**
 * Add a lexical error to the lexer without crashing the application.
 * This is done so that the user can get as much information as possible
 * about what went wrong, rather than terminating after a single error.
 * @param lexer the lexer to operate on
 * @param message the error message
 */
void Lexer_AddError(Lexer *lexer, string message);

bool Lexer_IsAtEnd(Lexer *lexer);

char Lexer_Consume(Lexer *lexer);

char Lexer_Peek(Lexer *lexer);

void Lexer_AddToken(Lexer *lexer, TokenType type, double value);

void Lexer_ParseToken(Lexer *lexer);

Token *Lexer_Tokenize(Lexer *lexer);

#endif