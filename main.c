#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Store in dst a substring of src from start (inclusive) to end (exclusive)
#define substring(dst, src, start, end) strncpy((dst), &(src)[(start)], (end) - (start))

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
    string lexeme;
    union {
        double value;
    };
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
    string source;
    size_t current;
    size_t start;
    Token *tokens;
    size_t token_count;
    size_t token_capacity;
    bool had_error;
    LexicalErrorList errors;
} Lexer;

string TokenType_ToString(TokenType type) {
    switch (type) {
        case TK_PLUS : return "PLUS";
        case TK_MINUS: return "MINUS";
        case TK_STAR: return "STAR";
        case TK_SLASH: return "SLASH";
        case TK_LPAREN: return "LPAREN";
        case TK_RPAREN: return "RPAREN";
        case TK_NUMBER: return "NUMBER";
    }
    return NULL;
}

/**
 * A constructor for the Lexer struct. Accepts a pointer to an
 * instance of a lexer, in which the default state of a lexer will
 * be written. That is `*lexer` will be a fresh lexer instance.
 * @param lexer the address of an empty lexer instance to write to
 */
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
        Lexer_Init(&lexer, line);
        Token *tokens = Lexer_Tokenize(&lexer);

        for (int i = 0; i < lexer.token_count; i++) {
            Token *token = &lexer.tokens[i];
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