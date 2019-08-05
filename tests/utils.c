#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <cmocka.h>
#include <gc.h>

#include "conf.h"

bool _dtor_1(void* ctx) {
    function_called();
    return true;
}

bool _dtor_2(void* ctx) {
    function_called();
    return true;
}

bool _dtor_3(void* ctx) {
    function_called();
    return true;
}

bool _dtor_failed(void *ctx) {
    function_called();
    return false;
}

void __mock_exit(int status) {
    check_expected(status);
    function_called();
}

size_t __mock_fprintf(FILE* fd, const char* format, ...) {
    check_expected(fd);
    function_called();
    return 0;
}

void setup(conf_t* conf) {
    gcinit(gc);

    for (int i = 0; conf->calls[i].fn != NULL; ++i) {
        gccall_t* c = &conf->calls[i];
        c->fn(&gc, c->del, c->ptr);
    }

    gc.error = conf->error;
}
