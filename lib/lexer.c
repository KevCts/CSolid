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

lexeme scan_lexeme() {
    lexer.start = lexer.current;
    if (end_of_file()) return make_lexeme(LEXEME_EOF);
    return error_lexeme("Expected character");
}
