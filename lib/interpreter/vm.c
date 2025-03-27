#include "vm.h"
#include "byte_code.h"

virtual_machine vm;

static interpret_result run() {

}

interpret_result interpret(const char *source) {
    byte_code* code;
    if (!compile(source, code))
        return INTERPRET_COMPILE_ERROR;
    vm.code = code;
    vm.ip = vm.code->code;

    interpret_result result = run();
    return result;
}
