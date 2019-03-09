/*
 * Created 190307+0800 lynnl
 */

#ifndef RAX_LPM_H
#define RAX_LPM_H

#include "rax/rax.h"
#include "utils.h"

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
    ssize_t *pos);

/**
 * Find longest prefix match in a radix tree
 *
 * @tree        the tree
 * @key         key to match
 * @len         length of the key
 * @pos         [OUT] position(index) in the key
 *              ranged [-1, len]
 *              -1          indicate mismatch(will return raxNotFound)
 *              (0, len)    indicate a submatch
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
        ssize_t *pos,
        void **data);

#endif

