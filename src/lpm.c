/*
 * Created 190307+0800 lynnl
 */

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
        ssize_t * __nullable lpmlen,
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

    if (h->iskey) {
        if (lpmnode) *lpmnode = h;
        if (lpmlen)  *lpmlen = i;
    }

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

/**
 * Find longest prefix match in a radix tree
 *
 * @tree        the tree
 * @key         key to match
 * @len         length of the key
 * @pos         [OUT] position(index) in the key
 *              ranged [-1, len]
 *              -1          indicate mismatch(will return 0)
 *              [0, len)    indicate a submatch
 *              len         indicate a full match
 * @data        [OUT] value associated to the longest prefix match node if found
 * @return      1 if found any match  0 o.w.
 *
 * see: rax/rax.c#raxFind
 */
int raxLongestPrefixMatch2(
        rax *tree,
        unsigned char *key,
        size_t len,
        ssize_t * __nullable pos,
        void * __nullable * __nullable data)
{
    raxNode *p = NULL;
    debugf("### Longest prefix match: %.*s len: %zu\n", (int) len, key, len);

    if (pos != NULL) *pos = -1;

    (void) raxLowWalk2(tree, key, len, &p, pos, NULL, NULL, NULL, NULL);

    if (p == NULL) {
        if (pos != NULL) ASSERT(*pos == -1);
        return 0;
    }

    if (pos != NULL) ASSERT(*pos != -1);
    if (data != NULL) *data = raxGetData(p);
    return 1;
}

/**
 * Find longest prefix match in a radix tree
 *
 * @tree        the tree
 * @key         key to match
 * @len         length of the key
 * @pos         [OUT] position(index) in the key
 *              ranged [-1, len]
 *              -1          indicate mismatch(will return raxNotFound)
 *              [0, len)    indicate a submatch
 *              len         indicate a full match
 * @return      value associated to the longest prefix match node
 *              raxNotFound if not match
 *
 * see: rax/rax.c#raxFind
 */
void *raxLongestPrefixMatch(
        rax *tree,
        unsigned char *key,
        size_t len,
        ssize_t * __nullable pos)
{
    void *data = raxNotFound;
    int found = raxLongestPrefixMatch2(tree, key, len, pos, &data);
    if (found) ASSERT(data != raxNotFound);
    return data;
}

