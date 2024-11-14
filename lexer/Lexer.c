#include "Lexer.h"
#include <ctype.h>

void Lexer_Init(Lexer *lexer, string source) {
    lexer->source = source;
    lexer->token_count = 0;
    lexer->token_capacity = 16;
    lexer->tokens = (Token *) calloc((lexer->token_capacity + 1), sizeof(Token));
    lexer->current = 0;
    lexer->start = 0;
    lexer->had_error = false;
    lexer->errors = (LexicalErrorList) { .first = NULL, .last = NULL };
}

void Lexer_AddError(Lexer *lexer, string message) {
    LexicalError *error = (LexicalError *) malloc(sizeof(LexicalError));
    *error = (LexicalError) { 
        .next = NULL,
        .message = message
    };

    if (lexer->errors.first == NULL) {
        lexer->errors.first = lexer->errors.last = error;
        lexer->had_error = true;
        return;
    }

    lexer->errors.last->next = error;
    lexer->errors.last = error;

    lexer->had_error = true;
}

bool Lexer_IsAtEnd(Lexer *lexer) {
    return lexer->current >= strlen(lexer->source);
}

char Lexer_Consume(Lexer *lexer) {
    return lexer->source[lexer->current++];
}

char Lexer_Peek(Lexer *lexer) {
    if (Lexer_IsAtEnd(lexer)) {
        return '\0';
    }
    return lexer->source[lexer->current];
}

void Lexer_AddToken(Lexer *lexer, TokenType type, double value) {
    string lexeme = (string) calloc((lexer->current - lexer->start + 1), sizeof(char));
    substring(lexeme, lexer->source, lexer->start, lexer->current);

    if (lexer->token_count >= lexer->token_capacity) {
        lexer->token_capacity += 16;
        lexer->tokens = (Token *) realloc(lexer->tokens, lexer->token_capacity * sizeof(Token));
    }

    lexer->tokens[lexer->token_count++] = (Token) { .type = type, .lexeme = lexeme, .value = value };
}

void Lexer_ParseToken(Lexer *lexer) {
    char ch = Lexer_Consume(lexer);
    if (isspace(ch)) return;
    switch (ch) {
        case '+': Lexer_AddToken(lexer, TK_PLUS, 0.00); break;
        case '-': Lexer_AddToken(lexer, TK_MINUS, 0.00); break;
        case '*': Lexer_AddToken(lexer, TK_STAR, 0.00); break;
        case '/': Lexer_AddToken(lexer, TK_SLASH, 0.00); break;
        case '(': Lexer_AddToken(lexer, TK_LPAREN, 0.00); break;
        case ')': Lexer_AddToken(lexer, TK_RPAREN, 0.00); break;
        default:  Lexer_AddError(lexer, "Invalid Token"); break;
    }
}

Token *Lexer_Tokenize(Lexer *lexer) {
    while (!Lexer_IsAtEnd(lexer)) {
        Lexer_ParseToken(lexer);
        lexer->start = lexer->current;
    }
    return lexer->tokens;
}