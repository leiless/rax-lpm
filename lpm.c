/*
 * Created 190307+0800 lynnl
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "lpm.h"
#include "utils.h"
#include "rax_internal.h"
#include "rax/rax.h"

/**
 * @lpmnode     Long prefix match node
 * @lpmlen      Long prefix match length in `s'
 * @return      How many bytes comsumed in `s'
 * see: rax/rax.c#raxLowWalk
 */
static inline size_t raxLowWalk2(
        rax *rax,
        register unsigned char *s,
        size_t len,
        raxNode * __nullable * __nullable lpmnode,
        size_t * __nullable lpmlen,
        raxNode * __nullable * __nullable stopnode,
        raxNode * __nullable * __nullable * plink,
        int * __nullable splitpos,
        raxStack * __nullable ts)
{
    raxNode **parentlink;
    raxNode *h;

    register size_t i = 0; /* Position in the string */
    register size_t j = 0; /* Position in the children node (or bytes if compressed) */
    register unsigned char *v;
    raxNode **children;

    ASSERT_NONNULL(rax);
    ASSERT_NONNULL(s);

    parentlink = &rax->head;
    h = *parentlink;

    while(h->size && i < len) {
        debugnode("Lookup current node", h);
        v = h->data;

        if (h->iscompr) {
            for (j = 0; j < h->size && i < len; j++, i++) {
                if (v[j] != s[i]) break;
            }
            if (j != h->size) break;
        } else {
            /*
             * Even when h->size is large, linear scan provides good
             * performances compared to other approaches that are in theory
             * more sounding, like performing a binary search
             */
            for (j = 0; j < h->size; j++) {
                if (v[j] == s[i]) break;
            }
            if (j == h->size) break;
            i++;
        }

        if (ts) (void) raxStackPush(ts, h);     /* Save stack of parent nodes */
        children = raxNodeFirstChildPtr(h);
        if (h->iscompr) j = 0;  /* Compressed node only child is at index 0 */
        (void) memcpy(&h, children + j, sizeof(h));
        parentlink = children + j;
        /*
         * If the new node is compressed and we do not
         * iterate again (since i == l) set the split position to 0
         * to signal this node represents the searched key
         */
        j = 0;

        /*
         * rax->head->iskey must be zero  see: raxNew()
         * .: we can safely check after h updated
         */
        if (h->iskey) {
            if (lpmnode) *lpmnode = h;
            if (lpmlen)  *lpmlen = i;
        }
    }

    debugnode("Lookup stop node is", h);

    if (stopnode) *stopnode = h;
    if (plink) *plink = parentlink;
    if (splitpos && h->iscompr) *splitpos = (int) j;

    return i;
}

int main(void)
{

    return 0;
}

