#include "Token.h"

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