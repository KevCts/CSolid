#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>
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
                lexer.start++;
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

static char* to_lower(const char* str, int length) {
    char* result = malloc(length * sizeof(char));
    for (int i = 0; i < length; i++)
        result[i] = tolower(str[i]);
    return result;
}

static lexeme_type keyword(int begin, int length, char* rest_to_check, lexeme_type objective) {
    if(lexer.current - lexer.start == length + begin) {
        char* lowercase = to_lower(lexer.start + begin, length); 
        if (memcmp(lowercase, rest_to_check, length) == 0) {
            free(lowercase);
            return objective;
        }
        free(lowercase);
    }
    return LEXEME_LITTERAL;
}

static lexeme_type litteral_type() {
    switch (tolower(lexer.start[0])) {
        case 'a':
            return keyword(1, 0, "", LEXEME_A);
        case 'b':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'a':
                        return keyword(2, 1, "r", LEXEME_BAR);
                    case 'o':
                        return keyword(2,3,"und", LEXEME_BOUND);
                }
            }
        case 'e':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'l':
                        return keyword(2, 3,"ist", LEXEME_ELIST);
                    case 'x':
                        return keyword(2, 0, "", LEXEME_EX);
                    case 'y':
                        return keyword(2, 0, "", LEXEME_EY);
                    case 'z':
                        return keyword(2, 0, "", LEXEME_EZ);
                }
            }
        case 'm':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'a':
                        return keyword(2, 1, "t", LEXEME_MAT);
                    case 'l':
                        return keyword(2, 3, "ist", LEXEME_MLIST);
                }
            }
        case 'n':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'l':
                        return keyword(2, 3, "ist", LEXEME_NLIST);
                    default:
                        return LEXEME_LITTERAL;
                }
            }
            return keyword(1, 0, "", LEXEME_N);
        case 'r':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'x':
                        return keyword(2, 0, "", LEXEME_RX);
                    case 'y':
                        return keyword(2, 0, "", LEXEME_RY);
                    case 'z':
                        return keyword(2, 0, "", LEXEME_RZ);
                }
            }
        case 's':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'e':
                        return keyword(2, 1, "c", LEXEME_SEC);
                    case 'l':
                        return keyword(2, 3, "ist", LEXEME_SLIST);
                }
            }
        case 'u':
            if (lexer.current - lexer.start >  1) {
                switch (tolower(lexer.start[1])) {
                    case 'x':
                        return keyword(2, 0, "", LEXEME_UX);
                    case 'y':
                        return keyword(2, 0, "", LEXEME_UY);
                    case 'z':
                        return keyword(2, 0, "", LEXEME_UZ);
                }
            }
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
        case '\n':
            lexer.line++;
            return make_lexeme(LEXEME_EOL);
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
