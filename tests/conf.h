#ifndef STDX_GC_TEST_CONF_H
#define STDX_GC_TEST_CONF_H

#include <stdlib.h>
#include <gc.h>

typedef void(gcfn_t)(__gc_t*, dtor_t, void*);

typedef struct {
    gcfn_t* fn;
    dtor_t* del;
    void*   ptr;
} gccall_t;

typedef struct {
    bool      error;
    gccall_t* calls;
} conf_t;

#endif // STDX_GC_TEST_CONF_H
