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
    data = raxLongestPrefixMatch(rax, (u8 *) "", 1, &sz);
    ASSERT(data == (void *) cookie);
    ASSERT(sz == 0);

    raxFree(rax);
}

int main(void)
{
    test1();
    test2();
    return 0;
}
