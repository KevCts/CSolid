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

lexeme scan_lexeme() {
    lexer.start = lexer.current;

    skip_whitespace();
    
    if (end_of_file()) return make_lexeme(LEXEME_EOF);

    char c = scan_next_letter();

    if (is_numeric(c)) return number_lexeme();
    
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
        default:
            break;
    }

    return error_lexeme("Expected character");
}
