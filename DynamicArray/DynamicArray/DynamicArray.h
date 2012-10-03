#ifndef DARRAY_H
#define DARRAY_H
#include <stddef.h>
#if __STDC_VERSION__ >= 199901L
    #include <stdbool.h>
#elif !defined(DARRAY_DISABLE_BOOL)
    /*
        If another library defines a boolean type then DARRAY_DISABLE_BOOL
        can be defined to disable this library's definition.
    */
    #define true  1
    #define false 0
    typedef int bool;
#endif
/*
    @description:
        Defines a type-dependent wrapper around darray_get 
        to avoid ugly casting and dereferencing when a
        stored value is needed.
*/
#define darray_getval(p,pos,type) (*(type*)darray_get(p, pos))
/*
    @description:
        Defines a type-independent wrapper around darray_set
        to correspond with darray_getval.
        Note: This macro isn't 100% comparable to darray_set.
            1) darray_setval cannot be used in an expression.
            2) darray_setval will not return a value like darray_set.
*/
#define darray_setval(p,pos,item,type) \
    do {                               \
        type dummy = item;             \
        darray_set(p, pos, &dummy);    \
    } while(false)
typedef struct darray darray;
struct darray {
    size_t item_size;    /* Size in bytes of each item (homogeneous) */
    size_t capacity;     /* Number of valid items possible without a resize */
    unsigned char *data; /* Storage for array items */
};
extern void  darray_init(darray *p, size_t item_size, size_t capacity);
extern void  darray_free(darray *p);
extern bool  darray_resize(darray *p, size_t new_size);
extern void *darray_get(const darray *p, size_t pos);
extern bool  darray_set(darray *p, size_t pos, void *item);
#endif /* DARRAY_H */