/*
 * Created 190309+0800 lynnl
 */

#ifndef RAX_INTERNAL_H
#define RAX_INTERNAL_H

#include "rax/rax.h"

extern void raxDebugShowNode(const char *, raxNode *);

/* Turn debugging messages on/off by compiling with RAX_DEBUG_MSG macro on.
 * When RAX_DEBUG_MSG is defined by default Rax operations will emit a lot
 * of debugging info to the standard output, however you can still turn
 * debugging on/off in order to enable it only when you suspect there is an
 * operation causing a bug using the function raxSetDebugMsg(). */
#ifdef RAX_DEBUG_MSG
#define debugf(...)                                                            \
    if (raxDebugMsg) {                                                         \
        printf("%s:%s:%d:\t", __FILE__, __FUNCTION__, __LINE__);               \
        printf(__VA_ARGS__);                                                   \
        fflush(stdout);                                                        \
    }

#define debugnode(msg,n) raxDebugShowNode(msg,n)
#else
#define debugf(...)
#define debugnode(msg,n)
#endif

/*
 * Return the padding needed in the characters
 *  section of a node having size 'nodesize'
 * The padding is needed to store the child pointers to aligned addresses
 * Note that we add 4 to the node size because the node has a four bytes header
 */
#define raxPadding(nodesize)    \
    ((sizeof(void *) - ((nodesize + 4) % sizeof(void *))) & (sizeof(void *) - 1))

/* Return the pointer to the first child pointer */
#define raxNodeFirstChildPtr(n) \
    ((raxNode **) ((n)->data + (n)->size + raxPadding((n)->size)))

/* Get the node auxiliary data */
extern void *raxGetData(raxNode *);

/**
 * Push an item into the rax stack
 * @returns     1 on success  0 on out of memory
 */
extern int raxStackPush(raxStack *, void *);

#endif  /* RAX_INTERNAL_H */

