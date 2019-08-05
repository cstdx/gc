/**
 * Copyright (C) 2019 cstdx/gc authors.
 * 
 * This file is part of cstdx/gc which is released under MIT.
 * See file LICENSE for full license details.
 */
#ifndef STDX_GC_H
#define STDX_GC_H

#include <stdbool.h>

static const int GC_FATAL = -1;

/**
 * Create new empty garbage collector.
 * 
 * The garbage collector is a stack of `dtor_t` functions which will be called
 * on order reverse they was added when the caller function returns. If a
 * destructor returns `false` the process will be terminated with the staus
 * GC_FATAL. The error message will be recorded to the `stderr`.
 */
#define gcinit(gc) __attribute__ ((__cleanup__(__gccleanup))) __gc_t gc = \
    { .stack = NULL, .error = false }

/**
 * Represents a desctructor interface.
 * 
 * @param  ptr Pointer to data to be deallocated.
 * @return Flag indicating the destruction completed successfully.
 */
typedef bool(dtor_t)(void* ptr);

typedef struct __gc_node __gc_node_t;

typedef struct {
    __gc_node_t* stack;
    bool         error;
} __gc_t;

/**
 * Push destructor to GC stack.
 * 
 * Destructor will be invoked when the caller functions exits.
 * 
 * @param gc  Garbage collector instance.
 * @param del Destructor.
 * @param ptr Pointer to be deallocated.
 */
void gcdefer(__gc_t* gc, dtor_t* del, void* ptr);

/**
 * Push destructor to GC stack.
 * 
 * Destructor will be invoked only if the GC instance has enabled error flag.
 * 
 * @param gc  Garbage collector instance.
 * @param del Destructor.
 * @param ptr Pointer to be deallocated.
 */
void gconerr(__gc_t* gc, dtor_t* del, void* ptr);

void __gccleanup(void* p);

#endif // STDX_GC_H
