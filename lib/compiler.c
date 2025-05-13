#include "compiler.h"
#include "lexer.h"
#include "value.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    lexeme current;
    lexeme previous;
    chunk* code;
    bool had_error;
    bool panic_mode;
} parser;

parser compiler;

typedef enum {
    PREC_TERM,
    PREC_FACTOR,
} precedence;

typedef void (*parse_fn)();

typedef struct {
    parse_fn prefix;
    parse_fn infix;
    precedence prec;
} parse_rule;

static void error(const char* message);

static void emit_byte(op_code byte) {
    write_chunk(compiler.code, byte, compiler.previous.line);
}

static uint8_t make_constant(value cst) {
    int result = add_value_to_array(cst, compiler.code->constants);
    if (result > UINT8_MAX) {
        error("Too much constants in one chunk.");
        return 0;
    }
        return (uint8_t)result;

}

static void emit_constant(value cst) {
    emit_byte(OP_CONSTANT);
    emit_byte(make_constant(cst));
}

void number() {
    emit_constant(NUMBER_VALUE(strtod(compiler.current.start, NULL)));
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

    for (;;) {
        compiler.current = scan_lexeme();
        if (compiler.current.type != LEXEME_ERROR) break;
        error(compiler.current.start);
    }
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
