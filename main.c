#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/interpreter/vm.h"

static void repl() {
    char line[1024];
    for(;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char* load_file(char* path){
    FILE* file = fopen(path, "r");

    fseek(file, 0L, SEEK_END);
    size_t fileLength = ftell(file);
    rewind(file);

    char* buffer = malloc(fileLength * sizeof(char) + 1);
    
    if(buffer == NULL){
        fprintf(stderr, "Not enough memory to read file \"%s\".\n", path);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), fileLength, file);

    if(bytes_read != fileLength){
        fprintf(stderr, "Could not open file \"%s\".", path);
        exit(74);
    }

    fclose(file);
    buffer[bytes_read] = '\0';
    return buffer;
}

static void run_file(char* path){
    char* source = load_file(path);

    interpret_result result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, char* argv[]) {
    init_vm();

    if (argc == 1){
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "Usage : csolid [path]\n");
        exit(65);
    }

    free_vm();

    return EXIT_SUCCESS;
}
