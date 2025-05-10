#include "compiler.h"
#include "lexer.h"
#include <stdio.h>

typedef struct {
    lexeme current;
    lexeme previous;
    chunk* code;
    bool had_error;
    bool panic_mode;
} parser;

parser compiler;

static void emit_byte(op_code byte) {
    write_chunk(compiler.code, byte, compiler.previous.line);
}

static void compile_end() {
    emit_byte(OP_RETURN);
}

static void error(const char* message) {
    if (compiler.panic_mode) return;
    compiler.had_error = true;

    fprintf(stderr, "[Line %d] Error", compiler.previous.line);

    switch (compiler.previous.type) {
        case LEXEME_EOF:
            fprintf(stderr, " at end");
            break;
        case LEXEME_ERROR:
            break;
        default:
            fprintf(stderr, " at '%.*s'", compiler.previous.length, compiler.previous.start);
    }

    fprintf(stderr, " : %s.\n", message);
}

static void parse_next_lexeme() {
    compiler.previous = compiler.current;

        compiler.current = scan_lexeme();
        if (compiler.current.type == LEXEME_ERROR)
            error(compiler.current.start);
}

static void consume_lexeme(lexeme_type lexeme_to_consume, const char* message) {
    if (compiler.current.type == lexeme_to_consume) {
        parse_next_lexeme();
        return;
    }

    error(message);
}

bool compile(const char* source, chunk* code) {
    init_lexer(source);

    compiler.code = code;
    compiler.had_error = false;
    compiler.panic_mode = false;

    parse_next_lexeme();
    consume_lexeme(LEXEME_EOF, "EXPECTED END OF EXPRESSION.");

    compile_end();
    return !compiler.had_error;
}
