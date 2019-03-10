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
        ssize_t * __nullable pos);

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
        void * __nullable * __nullable data);
```

The `__nullable` keyword annotates that left hand side type can be `NULL`, i.e. it's optional. by default, pointer type shouldn't be `NULL`.

Those two APIs used in different scenarios, they're the same per se, choose whichever you like.

# Caveats

When you insert any key into the radix tree in the following form:

```c
raxInsert(rax, (unsigned char *) _whatever1_, /* n.b. */ 0, (void *) _whatever2_, NULL);
```

e.g. the `len` parameter equals to zero, there'll always be a zero-length longest prefix match.

Thus, following longest prefix match assertion will always proceed:

```c
/* Assume `rax' contains only the zero-length key */
ssize_t sz;
void *data = raxLongestPrefixMatch(rax, (unsigned char *) "foobar", 6, &sz);
assert(data == (void *) _whatever2_);
assert(sz == 0);
```

### Rationale behind

It simply because when you insert a zero-length key, `rax->head->iskey` always yields 1, hence there is a match.

Similar situations applys to [`raxFind`](https://github.com/antirez/rax#key-lookup), [`raxRemove`](https://github.com/antirez/rax#deleting-keys):

```c
int ok;
void *data;

ok = raxInsert(rax, (unsigned char *) _whatever1_, /* n.b. */ 0, (void *) _whatever2_, NULL);
assert(ok == 1);

data = raxFind(rax, (unsigned char *) _whatever3_, 0);
assert(data == (void *) _whatever2_);
ok = raxRemove(rax, (unsigned char *) _whatever4_, 0, NULL);
assert(ok == 1);
```

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

[sic] [Rax](https://github.com/antirez/rax) is an open source project, released under the [BSD two clause license](https://github.com/antirez/rax/blob/master/COPYING).

rax-lpm is also an open source project, released under the [BSD 3-clause license](LICENSE).

<br>

*Created 180309+0800*
