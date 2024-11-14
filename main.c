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