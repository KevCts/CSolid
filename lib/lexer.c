#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    const char* start;
    const char* current;
    int line;
} scanner;

scanner lexer;

void init_lexer(const char* source) {
    lexer.current = source;
    lexer.start = source;
    lexer.line = 1;
}

static lexeme make_lexeme(lexeme_type type) {
    lexeme result;
    result.type = type;
    result.line = lexer.line;
    result.start = lexer.start;
    result.length = lexer.current - lexer.start;
    return result;
}

static bool end_of_file(){
    return *lexer.current == '\0';
}

static lexeme error_lexeme(const char* message) {
    lexeme result;
    result.type = LEXEME_ERROR;
    result.line = lexer.line;
    result.start = message;
    result.length = (int)strlen(message);
    return result;
}

static char see_next_letter() {
    return *lexer.current;
}

static char see_next_next_letter() {
    if (end_of_file()) return '\0';
    return lexer.current[1];
}

static char scan_next_letter() {
    lexer.current++;
    return lexer.current[-1];
}

static bool is_numeric(char c){
    return (c >= '0' && c <= '9') || (c == '.' && (see_next_next_letter() >= '0' && see_next_next_letter() <= '9'));
}

static lexeme number_lexeme() {
    while (is_numeric(see_next_letter()))
        scan_next_letter();

    if (see_next_letter() == '.' && (see_next_next_letter() >= '0' && see_next_next_letter() <= '9'))
        scan_next_letter();

    while (is_numeric(see_next_letter()))
        scan_next_letter();

    if (see_next_letter() == 'E' || see_next_letter() == 'e') {
        scan_next_letter();
        if ((see_next_letter() == '+' || see_next_letter() == '-') && is_numeric(see_next_next_letter()))
            scan_next_letter();
        while (is_numeric(see_next_letter())) {
            scan_next_letter();
        }
    }
    return make_lexeme(LEXEME_NUMBER);
}

static void skip_whitespace() {
    for (;;) {
        char c = see_next_letter();

        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                scan_next_letter();
                break;
            case '\n':
                lexer.line++;
                scan_next_letter();
                break;
            default:
                return;
        }
    }
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static lexeme_type keyword(int begin, int length, char* rest_to_check, lexeme_type objective) {
    if (lexer.current - lexer.start == length + begin && memcmp(lexer.start + begin, rest_to_check, length) == 0) return objective;

    return LEXEME_LITTERAL;
}

static lexeme_type litteral_type() {
    switch (lexer.start[0]) {
        case 'n':
        case 'N':
            if (lexer.current - lexer.start >  1) {
                switch (lexer.start[1]) {
                    case 'l':
                    case 'L':
                        return keyword(2, 3, "ist", LEXEME_NLIST);
                }
            }
            return keyword(1, 0, "", LEXEME_N);
        default:
            return LEXEME_LITTERAL;
    }
}

static lexeme litteral_lexeme() {
    while (is_alpha(see_next_letter()) || is_numeric(see_next_letter())) scan_next_letter();

    return make_lexeme(litteral_type());
}

lexeme scan_lexeme() {
    lexer.start = lexer.current;

    skip_whitespace();

    if (end_of_file()) return make_lexeme(LEXEME_EOF);

    char c = scan_next_letter();

    if (is_numeric(c)) return number_lexeme();

    if (is_alpha(c)) return litteral_lexeme();

    switch (c) {
        case '(':
            return make_lexeme(LEXEME_LEFT_PAREN);
        case ')':
            return make_lexeme(LEXEME_RIGHT_PAREN);
        case '+':
            return make_lexeme(LEXEME_PLUS);
        case '-':
            return make_lexeme(LEXEME_MINUS);
        case '*':
            return make_lexeme(LEXEME_STAR);
        case '/':
            return make_lexeme(LEXEME_SLASH);
        case '[':
            return make_lexeme(LEXEME_LEFT_BRACE);
        case ']':
            return make_lexeme(LEXEME_RIGHT_BRACE);
        default:
            break;
    }

    return error_lexeme("Expected character");
}
