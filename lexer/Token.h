#ifndef TOKEN_H
#define TOKEN_H

#include "../util/string.h"
#include "stdlib.h"

// Types of tokens to accept
typedef enum {
    TK_PLUS, TK_MINUS, TK_STAR, TK_SLASH,
    TK_LPAREN, TK_RPAREN,
    TK_NUMBER
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    string lexeme;
    union {
        double value;
    };
} Token;


string TokenType_ToString(TokenType type);
void Token_CleanUp(Token *token);


#endif