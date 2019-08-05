#include <stdlib.h>
#include <stdio.h>

#include "gc.h"

#ifdef STDX_GC_TEST

extern void   __mock_exit(int);
extern size_t __mock_fprintf(FILE*, const char*, ...);

#define exit    __mock_exit
#define fprintf __mock_fprintf

#endif //STDX_GC_TEST

typedef struct __gc_node {
    struct __gc_node* next;
    dtor_t* del;
    void*   ptr;
    bool    err;
} __gc_node_t;

void __gcpush(__gc_t* gc, dtor_t* del, void* ptr, bool err) {
    __gc_node_t* node = (__gc_node_t*)malloc(sizeof(__gc_node_t));
    if (node == NULL) {
        fprintf(stderr, "fatal: %s\n", "unable allocate memory for gc node");
        exit(GC_FATAL);
    }

    node->next = gc->stack;
    node->del = del;
    node->ptr = ptr;
    node->err = err;

    gc->stack = node;
}

void __gcdel(__gc_node_t* node) {
    if (!(node->del)(node->ptr)) {
        fprintf(stderr, "fatal: %s\n", "destructor failed");
        exit(GC_FATAL);
    }
}

void __gccleanup(void* p) {
    __gc_t* gc = (__gc_t*)p;

    while (gc->stack) {
        __gc_node_t* node = gc->stack;
        __gc_node_t* next = node->next;

        if (node->err) {
            if (gc->error) {
                __gcdel(node);
            }
        } else {
            __gcdel(node);
        }

        free(node);
        gc->stack = next;
    }
}

void gcdefer(__gc_t* gc, dtor_t* del, void* ptr) {
    __gcpush(gc, del, ptr, false);
}

void gconerr(__gc_t* gc, dtor_t* del, void* ptr) {
    __gcpush(gc, del, ptr, true);
}
