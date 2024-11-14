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

/**
 * Add a lexical error to the lexer without crashing the application.
 * This is done so that the user can get as much information as possible
 * about what went wrong, rather than terminating after a single error.
 * @param lexer the lexer to operate on
 * @param message the error message
 */
static void Lexer_AddError(Lexer *lexer, string message) {
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

static bool Lexer_IsAtEnd(Lexer *lexer) {
    return lexer->current >= strlen(lexer->source);
}

static char Lexer_Consume(Lexer *lexer) {
    return lexer->source[lexer->current++];
}

static char Lexer_Peek(Lexer *lexer) {
    if (Lexer_IsAtEnd(lexer)) {
        return '\0';
    }
    return lexer->source[lexer->current];
}

static void Lexer_AddToken(Lexer *lexer, TokenType type, double value) {
    string lexeme = (string) calloc((lexer->current - lexer->start + 1), sizeof(char));
    substring(lexeme, lexer->source, lexer->start, lexer->current);

    if (lexer->token_count >= lexer->token_capacity) {
        lexer->token_capacity += 16;
        lexer->tokens = (Token *) realloc(lexer->tokens, lexer->token_capacity * sizeof(Token));
    }

    lexer->tokens[lexer->token_count++] = (Token) { .type = type, .lexeme = lexeme, .value = value };
}

static void Lexer_ParseNumber(Lexer *lexer) {
    while (!Lexer_IsAtEnd(lexer) && isdigit(Lexer_Peek(lexer))) {
        Lexer_Consume(lexer);
    }

    if (!Lexer_IsAtEnd(lexer) && Lexer_Peek(lexer) == '.') {
        Lexer_Consume(lexer); // consume the dot token
        while (!Lexer_IsAtEnd(lexer) && isdigit(Lexer_Peek(lexer))) {
            Lexer_Consume(lexer);
        }
    }

    string lexeme = (string) calloc(lexer->current - lexer->start + 1, sizeof(char));
    substring(lexeme, lexer->source, lexer->start, lexer->current);
    double value = strtod(lexeme, NULL);

    Lexer_AddToken(lexer, TK_NUMBER, value);
}

static void Lexer_ParseToken(Lexer *lexer) {
    char ch = Lexer_Consume(lexer);
    if (isspace(ch)) return;
    switch (ch) {
        case '+': Lexer_AddToken(lexer, TK_PLUS, 0.00); break;
        case '-': Lexer_AddToken(lexer, TK_MINUS, 0.00); break;
        case '*': Lexer_AddToken(lexer, TK_STAR, 0.00); break;
        case '/': Lexer_AddToken(lexer, TK_SLASH, 0.00); break;
        case '(': Lexer_AddToken(lexer, TK_LPAREN, 0.00); break;
        case ')': Lexer_AddToken(lexer, TK_RPAREN, 0.00); break;
        default: {
            if (isdigit(ch)) {
                Lexer_ParseNumber(lexer);
                break;
            }
            Lexer_AddError(lexer, "Invalid Token"); 
            break;
        }
    }
}

Token *Lexer_Tokenize(Lexer *lexer) {
    while (!Lexer_IsAtEnd(lexer)) {
        Lexer_ParseToken(lexer);
        lexer->start = lexer->current;
    }
    return lexer->tokens;
}