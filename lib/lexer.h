#ifndef CSOLID_LEXER_H
#define CSOLID_LEXER_H

typedef enum {
    LEXEME_EOL,

    LEXEME_BAR, LEXEME_BOUND,

    LEXEME_ELIST,

    LEXEME_LEFT_PAREN, LEXEME_RIGHT_PAREN,
    LEXEME_LEFT_BRACE, LEXEME_RIGHT_BRACE,

    LEXEME_EX, LEXEME_EY, LEXEME_EZ,

    LEXEME_MAT, LEXEME_MLIST,

    LEXEME_SEC, LEXEME_SLIST,

    LEXEME_A,

    LEXEME_UX, LEXEME_UY, LEXEME_UZ,
    
    LEXEME_RX, LEXEME_RY, LEXEME_RZ,

    LEXEME_PLUS, LEXEME_MINUS, LEXEME_STAR, LEXEME_SLASH,

    LEXEME_NUMBER,

    LEXEME_N, LEXEME_NLIST,

    LEXEME_LITTERAL,

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
