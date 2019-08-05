#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <cmocka.h>
#include <gc.h>

#include "conf.h"

extern bool _dtor_1(void*);
extern bool _dtor_2(void*);
extern bool _dtor_3(void*);
extern bool _dtor_failed(void*);
extern void setup(conf_t*);

static void test_gc_no_errors(void** state) {
    (void)state;

    gccall_t calls[] = {
        { .fn = NULL },
    };

    conf_t conf = {
        .error = false,
        .calls = calls,
    };

    setup(&conf);
}

static void test_gc_calls_destructors_in_reverse_order(void** state) {
    (void)state;

    expect_function_call(_dtor_3);
    expect_function_call(_dtor_2);
    expect_function_call(_dtor_1);

    gccall_t calls[] = {
        { .fn = gcdefer, .del = _dtor_1, .ptr = NULL },
        { .fn = gcdefer, .del = _dtor_2, .ptr = NULL },
        { .fn = gcdefer, .del = _dtor_3, .ptr = NULL },
        { .fn = NULL },
    };

    conf_t conf = {
        .error = false,
        .calls = calls,
    };

    setup(&conf);
}

static void test_gc_onerr_not_called_if_not_error(void** state) {
    (void)state;

    expect_function_call(_dtor_3);
    expect_function_call(_dtor_1);

    gccall_t calls[] = {
        { .fn = gcdefer, .del = _dtor_1, .ptr = NULL },
        { .fn = gconerr, .del = _dtor_2, .ptr = NULL },
        { .fn = gcdefer, .del = _dtor_3, .ptr = NULL },
        { .fn = NULL },
    };

    conf_t conf = {
        .error = false,
        .calls = calls,
    };

    setup(&conf);
}

static void test_gc_onerr_called_if_error(void** state) {
    (void)state;

    expect_function_call(_dtor_3);
    expect_function_call(_dtor_2);
    expect_function_call(_dtor_1);

    gccall_t calls[] = {
        { .fn = gcdefer, .del = _dtor_1, .ptr = NULL },
        { .fn = gconerr, .del = _dtor_2, .ptr = NULL },
        { .fn = gcdefer, .del = _dtor_3, .ptr = NULL },
        { .fn = NULL },
    };

    conf_t conf = {
        .error = true,
        .calls = calls,
    };

    setup(&conf);
}

static void test_gc_terminated_if_destructor_returned_false(void** state) {
    (void)state;

    expect_function_call(_dtor_3);
    expect_function_call(_dtor_failed);
    expect_function_call(__mock_fprintf);
    expect_function_call(__mock_exit);

    expect_value(__mock_fprintf, fd, stderr);
    expect_value(__mock_exit, status, GC_FATAL);

    gccall_t calls[] = {
        { .fn = gcdefer, .del = _dtor_failed, .ptr = NULL },
        { .fn = gcdefer, .del = _dtor_3, .ptr = NULL },
        { .fn = NULL },
    };

    conf_t conf = {
        .error = false,
        .calls = calls,
    };

    setup(&conf);
}

const struct CMUnitTest gc_tests[] = {
    cmocka_unit_test(test_gc_no_errors),
    cmocka_unit_test(test_gc_calls_destructors_in_reverse_order),
    cmocka_unit_test(test_gc_onerr_not_called_if_not_error),
    cmocka_unit_test(test_gc_onerr_called_if_error),
    cmocka_unit_test(test_gc_terminated_if_destructor_returned_false),
};

int main(void)
{
    return cmocka_run_group_tests(gc_tests, NULL, NULL);
}
