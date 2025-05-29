#ifndef CSOLID_LEXER_H
#define CSOLID_LEXER_H

typedef enum {
    LEXEME_EOL,

    LEXEME_LEFT_PAREN, LEXEME_RIGHT_PAREN,

    LEXEME_PLUS, LEXEME_MINUS, LEXEME_STAR, LEXEME_SLASH,

    LEXEME_NUMBER,

    LEXEME_EOF, LEXEME_ERROR
} lexeme_type;

typedef struct {
    lexeme_type type;
    const char* start;
    int line, length;
} lexeme;

void init_lexer(const char* source);
lexeme scan_lexeme();

#endif
