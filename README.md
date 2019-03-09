# rax-lpm, A radix tree with longest prefix match extension

rax-lpm derived from [rax](https://github.com/antirez/rax), it implemented a widely used [longest prefix match algorithm](https://en.wikipedia.org/wiki/Longest_prefix_match) inside the radix tree.

The primary goal of this extension is to implement the longest prefix match algorithm by providing [raxFind](https://github.com/antirez/rax#key-lookup)-like functionality without intervention of [rax iterators](https://github.com/antirez/rax#iterators). It's extremely useful when you do a path prefix match, or embed the [rax](https://github.com/antirez/rax) into embedded device by removing [rax iterators](https://github.com/antirez/rax#iterators).

For more information, please refers to [rax](https://github.com/antirez/rax) project.

# Longest prefix match API

[rax](https://github.com/antirez/rax) itself is a full-featured radix tree implementation, for basic API usage, please see [Basic API](https://github.com/antirez/rax#basic-api).

There're two APIs used for radix tree longest prefix match:

```c
/**
 * Find longest prefix match in a radix tree
 *
 * @tree        the tree
 * @key         key to match
 * @len         length of the key
 * @pos         [OUT] how many bytes processed(nullable)
 *              ranged [0, len]
 *              0           indicate mismatch(will return raxNotFound)
 *              (0, len)    indicate a submatch
 *              len         indicate a full match
 * @return      value associated to the longest prefix match node
 *              raxNotFound if not match
 */
void *raxLongestPrefixMatch(
        rax *tree,
        unsigned char *key,
        size_t len,
        size_t * __nullable pos);

/**
 * Find longest prefix match in a radix tree
 *
 * @tree        the tree
 * @key         key to match
 * @len         length of the key
 * @pos         [OUT] how many bytes processed
 *              ranged [0, len]
 *              0           indicate mismatch(will return raxNotFound)
 *              (0, len)    indicate a submatch
 *              len         indicate a full match
 * @data        [OUT] value associated to the longest prefix match node if found
 * @return      1 if found any match  0 o.w.
 */
int raxLongestPrefixMatch2(
        rax *tree,
        unsigned char *key,
        size_t len,
        size_t * __nullable pos,
        void * __nullable * __nullable data);
```

The `__nullable` keyword annotates that left hand side type can be `NULL`, i.e. it's optional. by default, pointer type shouldn't be `NULL`.

Those two APIs used in different scenarios, they're the same per se, choose whichever you like.

# Run `rax-lpm` tests

This project submoduled [rax](https://github.com/antirez/rax) source, so it can stay up-to-date automatically.

Before compile the source, you should first do:

```shell
git apply --directory rax rax.c.patch
```

To patch [rax/rax.c](https://github.com/antirez/rax/blob/master/rax.c)(only once)

And then run the tests:

```
# `release' target for release build
make
./test_lpm
```

# Debugging `rax-lpm`

Please see [Debugging Rax](https://github.com/antirez/rax#debugging-rax).

# License

[sic] [Rax](https://github.com/antirez/rax) is an open source project, released under the BSD two clause license.

rax-lpm is also an open source project, released under the [BSD 3-clause license](LICENSE).

---

*Created 180309+0800*
