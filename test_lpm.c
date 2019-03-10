/*
 * Created 190309+0800 lynnl
 */

#include <stdio.h>

#include "lpm.h"
#include "utils.h"

#ifndef u8
#define u8          unsigned char
#endif

#define HEX_MAGIC   0xfadefacedeadbeef

#define LOG(fmt, ...)   (void) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

static void test1(void)
{
    rax *rax = raxNew();
    ssize_t sz;
    void *data;

    ASSERT_NONNULL(rax);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) 0xdeadbeef, 0, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 0, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "0", 1, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ab", 2, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "xyz", 3, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "dead", 4, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "space", 5, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    raxFree(rax);
}

static void test2(void)
{
    rax *rax = raxNew();
    static uint64_t cookie = 0xfeedfacecafebeef;
    ssize_t sz;
    void *data;
    int ok;

    ASSERT_NONNULL(rax);

    ASSERT(rax->head->iskey == 0);
    ok = raxInsert(rax, (u8 *) "", 0, (void *) cookie, NULL);
    ASSERT(ok == 1);
    ASSERT(rax->head->iskey == 1);
    ASSERT(rax->head->iscompr == 0);
    ASSERT(rax->head->size == 0);

    data = raxFind(rax, (u8 *) "", 0);
    ASSERT(data == (void *) cookie);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 0, &sz);
    ASSERT(data == (void *) cookie);
    ASSERT(sz == 0);    /* Full match */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 1, &sz);
    ASSERT(data == (void *) cookie);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "0", 1, &sz);
    ASSERT(data == (void *) cookie);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "xy", 2, &sz);
    ASSERT(data == (void *) cookie);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abc", 3, &sz);
    ASSERT(data == (void *) cookie);
    ASSERT(sz == 0);

    raxFree(rax);
}

static void test3(void)
{
    rax *rax = raxNew();
    static char *uuid = "612A3D21-F94E-4EEE-9012-A877B96729EB";
    ssize_t sz;
    void *data;
    int ok;

    ASSERT_NONNULL(rax);

    ok = raxInsert(rax, (u8 *) "foo", 3, (void *) uuid, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "foobar", 6, (void *) uuid, NULL);
    ASSERT(ok == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 0, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 1, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "f", 1, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "fo", 2, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "foo", 2, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "foo", 3, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);        /* Full match */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "for", 3, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "fool", 4, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);        /* Submatch */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "foolish", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);        /* Submatch */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "fooba", 5, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "foobar", 6, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);        /* Full match */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "foobar!", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);        /* Submatch */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "foobar!phenomenon", 17, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);        /* Submatch */

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "#foobar", 8, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    raxFree(rax);
}

static void test4(void)
{
    rax *rax = raxNew();
    ASSERT_NONNULL(rax);
    static char *uuid = "6d9ab4ff-e733-4db7-9f0b-750e0338d239";
    ssize_t sz;
    void *data;
    int ok;

    ok = raxInsert(rax, (u8 *) "", 0, (void *) uuid, NULL);
    ASSERT(ok == 1);
    ok = raxInsert(rax, (u8 *) "a", 1, (void *) uuid, NULL);
    ASSERT(ok == 1);
    ok = raxInsert(rax, (u8 *) "ab", 2, (void *) uuid, NULL);
    ASSERT(ok == 1);
    ok = raxInsert(rax, (u8 *) "abc", 3, (void *) uuid, NULL);
    ASSERT(ok == 1);
    ok = raxInsert(rax, (u8 *) "abcd", 4, (void *) uuid, NULL);
    ASSERT(ok == 1);
    ok = raxInsert(rax, (u8 *) "abcde", 5, (void *) uuid, NULL);
    ASSERT(ok == 1);
    ok = raxInsert(rax, (u8 *) "abcdef", 6, (void *) uuid, NULL);
    ASSERT(ok == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 0, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "\0", 2, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "a", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "a", 2, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ab", 2, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 2);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abc", 3, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcd", 4, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 4);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcde", 5, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 5);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcdef", 6, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcdef", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcdef!", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcdef!", 8, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "\0abcdef", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    raxFree(rax);
}

static void test5(void)
{
    rax *rax = raxNew();
    ASSERT_NONNULL(rax);
    static char *uuid = "44eadd55-d769-4a3e-b7fe-f3fc58c72f31";
    ssize_t sz;
    void *data;
    int ok;

    ok = raxInsert(rax, (u8 *) "A", 1, (void *) uuid, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "ABC", 3, (void *) uuid, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "ABCDEF", 6, (void *) uuid, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "ABCDEFG", 7, (void *) uuid, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "ABCDEFGHIJ", 10, (void *) uuid, NULL);
    ASSERT(ok == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 0, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "a", 1, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "A", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "AB", 2, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABC", 3, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABD", 3, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCABC", 6, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCD", 4, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDE", 5, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 3);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEF", 6, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEFG", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 7);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEFg", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 6);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEFG!", 7, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 7);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEFGHI", 9, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 7);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEFGHIJ", 10, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 10);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "ABCDEFGHIJ0123456789", 20, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 10);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "\0ABC", 3, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    raxFree(rax);
}

static void test(void)
{
    rax *rax = raxNew();
    ASSERT_NONNULL(rax);
    raxFree(rax);
}

int main(void)
{
    test();
    test1();
    test2();
    test3();
    test4();
    test5();
    LOG("Built: %s %s  Pass!", __DATE__, __TIME__);
    return 0;
}

