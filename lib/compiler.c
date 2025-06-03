#include "compiler.h"
#include "chunk.h"
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
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
} precedence;

typedef void (*parse_fn)();

typedef struct {
    parse_fn prefix;
    parse_fn infix;
    precedence prec;
} parse_rule;

static void error(const char* message);
static parse_rule* get_parse_rule(lexeme_type lex_type);
static void parse_next_lexeme();

static void emit_byte(uint8_t byte) {
    write_chunk(compiler.code, byte, compiler.previous.line);
}

static uint8_t make_constant(value cst) {
    int result = add_value_to_array(cst, compiler.code->constants);
    if (result > UINT8_MAX) {
        error("Too much constants in one chunk");
        return 0;
    }
        return (uint8_t)result;

}

static void emit_constant(value cst) {
    emit_byte(OP_CONSTANT);
    emit_byte(make_constant(cst));
}

void number() {
    emit_constant(NUMBER_VALUE(strtod(compiler.previous.start, NULL)));
}

void parse_precedence(precedence prec) {
    parse_next_lexeme();

    parse_fn prefix_fn = get_parse_rule(compiler.previous.type)->prefix;

    if (prefix_fn == NULL) {
        error("Expected expression");
        return;
    }

    prefix_fn();

    while (prec <= get_parse_rule(compiler.current.type)->prec) {
        parse_next_lexeme();
        parse_fn infix_fn = get_parse_rule(compiler.previous.type)->infix;
        infix_fn();
    }
}

void unary() {
    lexeme_type operand = compiler.previous.type;

    parse_precedence(PREC_UNARY);

    switch (operand) {
        case LEXEME_MINUS :
            emit_byte(OP_NEGATE); break;
        default :
            return;
    }
}

void binary() {
    lexeme_type operand = compiler.previous.type;

    parse_precedence(get_parse_rule(operand)->prec);

    switch (operand) {
        case LEXEME_PLUS :
            emit_byte(OP_ADD); break;
        case LEXEME_MINUS :
            emit_byte(OP_SUBSTRACT); break;
        case LEXEME_STAR :
            emit_byte(OP_MULTIPLY); break;
        case LEXEME_SLASH :
            emit_byte(OP_DIVIDE); break;
        default :
            return;
    }
}

static void consume_lexeme(lexeme_type lexeme_to_consume, const char* message) {
    if (compiler.current.type == lexeme_to_consume) {
        parse_next_lexeme();
        return;
    }

    error(message);
}

static void grouping() {
    parse_precedence(PREC_ASSIGNMENT);
    consume_lexeme(LEXEME_RIGHT_PAREN, "EXPECTED \')\' NOT FOUND");
}

static void list() {
    emit_byte(OP_LIST_START);
    while (compiler.current.type != LEXEME_RIGHT_BRACE && compiler.current.type != LEXEME_EOF)
        parse_precedence(PREC_ASSIGNMENT);
    consume_lexeme(LEXEME_RIGHT_BRACE, "EXPECTED \']\' NOT FOUND");
    emit_byte(OP_LIST_END);
}

static void parse_arguments() {
    emit_constant(NIL_VALUE);
    do {
        parse_precedence(PREC_ASSIGNMENT);
    } while (compiler.current.type != LEXEME_EOL);
}

static void n() {
    parse_arguments();
    emit_byte(OP_N);
}

static void nlist(){
    emit_byte(OP_NLIST);
}

static void compile_end() {
    emit_byte(OP_RETURN);
}

static void nothing() {}

static void mat_prop() {
    switch (compiler.previous.type) {
        case LEXEME_EX:
            emit_constant(MAT_PROP_VALUE(EX));
            break;
        case LEXEME_EY:
            emit_constant(MAT_PROP_VALUE(EY));
            break;
        case LEXEME_EZ:
            emit_constant(MAT_PROP_VALUE(EZ));
            break;
        default:
            error("Invalid material property");
    }
}

static void mat() {
    parse_arguments();
    emit_byte(OP_MAT);
}

static void sec() {
    parse_arguments();
    emit_byte(OP_SEC);
}

static void mlist() {
    emit_byte(OP_MLIST);
}

static void slist() {
    emit_byte(OP_SLIST);
}

static void elist() {
    emit_byte(OP_ELIST);
}

static void sec_para() {
    switch (compiler.previous.type) {
        case LEXEME_A:
            emit_constant(SEC_PARA_VALUE(SEC));
            break;
        default:
            error("Invalid section parameter");
    }
}

static void element() {
    lexeme_type element_type = compiler.previous.type;
    parse_arguments();
    switch (element_type) {
        case LEXEME_BAR:
            emit_byte(OP_BAR);
            break;
    }
}

static void dir() {
    switch (compiler.previous.type) {
        case LEXEME_UX:
            emit_constant(DIR_VALUE(UX));
            break;
        case LEXEME_UY:
            emit_constant(DIR_VALUE(UY));
            break;
        case LEXEME_UZ:
            emit_constant(DIR_VALUE(UZ));
            break;
        case LEXEME_RX:
            emit_constant(DIR_VALUE(RX));
            break;
        case LEXEME_RY:
            emit_constant(DIR_VALUE(RY));
            break;
        case LEXEME_RZ:
            emit_constant(DIR_VALUE(RZ));
            break;
    }
}

static void bound() {
    parse_arguments();
    emit_byte(OP_BOUND);
}

parse_rule parse_rules[] = {
    [LEXEME_N] = {n, NULL, PREC_NONE},
    [LEXEME_NLIST] = {nlist, NULL, PREC_NONE},
    [LEXEME_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
    [LEXEME_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [LEXEME_LEFT_BRACE] = {list, NULL, PREC_NONE},
    [LEXEME_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [LEXEME_NUMBER]  = {number, NULL, PREC_NONE},
    [LEXEME_PLUS]  = {NULL, binary, PREC_TERM},
    [LEXEME_MINUS]  = {unary, binary, PREC_TERM},
    [LEXEME_STAR]  = {NULL, binary, PREC_FACTOR},
    [LEXEME_SLASH]  = {NULL, binary, PREC_FACTOR},
    [LEXEME_ERROR] = {NULL, NULL, PREC_NONE},
    [LEXEME_EOL] = {nothing, NULL, PREC_NONE},
    [LEXEME_EOF] = {NULL, NULL, PREC_NONE},
    [LEXEME_MAT] = {mat, NULL, PREC_NONE},
    [LEXEME_MLIST] = {mlist, NULL, PREC_NONE},
    [LEXEME_SEC] = {sec, NULL, PREC_NONE},
    [LEXEME_SLIST] = {slist, NULL, PREC_NONE},
    [LEXEME_EX] = {mat_prop, NULL, PREC_NONE},
    [LEXEME_EY] = {mat_prop, NULL, PREC_NONE},
    [LEXEME_EZ] = {mat_prop, NULL, PREC_NONE},
    [LEXEME_A] = {sec_para, NULL, PREC_NONE},
    [LEXEME_BAR] = {element, NULL, PREC_NONE},
    [LEXEME_ELIST] = {elist, NULL, PREC_NONE},
    [LEXEME_UX] = {dir, NULL, PREC_NONE},
    [LEXEME_UY] = {dir, NULL, PREC_NONE},
    [LEXEME_UZ] = {dir, NULL, PREC_NONE},
    [LEXEME_RX] = {dir, NULL, PREC_NONE},
    [LEXEME_RY] = {dir, NULL, PREC_NONE},
    [LEXEME_RZ] = {dir, NULL, PREC_NONE},
    [LEXEME_BOUND] = {bound, NULL, PREC_NONE},
};

static parse_rule* get_parse_rule(lexeme_type lex_type) {
    return &parse_rules[lex_type];
}


static void error(const char* message) {
    if (compiler.panic_mode) return;
    compiler.had_error = true;
    compiler.panic_mode = true;

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


static void build_op_code() {
    parse_precedence(PREC_ASSIGNMENT);
}

bool compile(const char* source, chunk* code) {
    init_lexer(source);

    compiler.code = code;
    compiler.had_error = false;
    compiler.panic_mode = false;

    do {
        parse_next_lexeme();
        if (compiler.current.type != LEXEME_EOF)
            build_op_code();
    } while (compiler.current.type != LEXEME_EOF);

    compile_end();
    return !compiler.had_error;
}
