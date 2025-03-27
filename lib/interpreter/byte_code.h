#ifndef CSOLID_BYTE_CODE
#define CSOLID_BYTE_CODE

typedef enum {
    OP_RETURN,
} op_code;

typedef struct {
    op_code* code;
} byte_code; 

#endif
