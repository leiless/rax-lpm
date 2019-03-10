/*
 * Created 190309+0800 lynnl
 */

#include <stdio.h>
#include <string.h>

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
    data = raxLongestPrefixMatch(rax, (u8 *) "x", 1, &sz);
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

static void test6(void)
{
    rax *rax = raxNew();
    ASSERT_NONNULL(rax);
    static char *uuid = "bbda30fa-a3b0-4fe8-b23c-a0d6c8f56ba9";
    ssize_t sz;
    void *data;
    int ok;

    ok = raxInsert(rax, (u8 *) "!", 0, (void *) uuid, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "", 1, (void *) uuid, NULL);
    ASSERT(ok == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 0, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) 0x90abcdef, 0, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "?", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "!", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "\0", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "", 1, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcd", 4, &sz);
    ASSERT(data == uuid);
    ASSERT(sz == 0);

    ok = raxRemove(rax, (u8 *) "@", 0, NULL);
    ASSERT(ok = 1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "abcd", 4, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    raxFree(rax);
}

static void test7(void)
{
    rax *rax = raxNew();
    ASSERT_NONNULL(rax);
    static char *uuid = "129fad82-16f2-4ef1-a143-d5027a81d252";
    void *data;
    int ok;

    ok = raxInsert(rax, (u8 *) "a", 0, (void *) uuid, NULL);
    ASSERT(ok == 1);
    data = raxFind(rax, (u8 *) "", 0);
    ASSERT(data == uuid);
    ok = raxRemove(rax, (u8 *) "A", 0, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "b", 0, (void *) uuid, NULL);
    ASSERT(ok == 1);
    data = raxFind(rax, (u8 *) "B", 0);
    ASSERT(data == uuid);
    ok = raxRemove(rax, (u8 *) "", 0, NULL);
    ASSERT(ok == 1);

    ok = raxInsert(rax, (u8 *) "", 0, (void *) uuid, NULL);
    ASSERT(ok == 1);
    data = raxFind(rax, (u8 *) "C", 0);
    ASSERT(data == uuid);
    ok = raxRemove(rax, (u8 *) "c", 0, NULL);
    ASSERT(ok == 1);

    raxFree(rax);
}

struct kv {
    void *data;
    u8 *key;
    size_t len;
};

struct kv test8_data[] = {
    {(void *) 0, (u8 *) "/usr/local/bin/brew", 19},
    {(void *) 1, (u8 *) "/usr/local/bin/git", 18},
    {(void *) 2, (u8 *) "/usr/local/bin/git-cvsserver", 28},
    {(void *) 3, (u8 *) "/usr/local/bin/git-lfs", 22},
    {(void *) 4, (u8 *) "/usr/local/bin/git-receive-pack", 31},
    {(void *) 5, (u8 *) "/usr/local/bin/git-shell", 24},
    {(void *) 6, (u8 *) "/usr/local/bin/git-upload-archive", 33},
    {(void *) 7, (u8 *) "/usr/local/bin/git-upload-pack", 30},
    {(void *) 8, (u8 *) "/usr/local/bin/gitk", 19},
    {(void *) 9, (u8 *) "/usr/local/bin/go", 17},
    {(void *) 10, (u8 *) "/usr/local/bin/godoc", 20},
    {(void *) 11, (u8 *) "/usr/local/bin/gofmt", 20},
    {(void *) 12, (u8 *) "/usr/local/bin/pcre2-config", 27},
    {(void *) 13, (u8 *) "/usr/local/bin/pcre2grep", 24},
    {(void *) 14, (u8 *) "/usr/local/bin/pcre2test", 24},
    {(void *) 15, (u8 *) "/usr/local/bin/pkg-config", 25},
    {(void *) 16, (u8 *) "/usr/local/Cellar/gettext", 25},
    {(void *) 17, (u8 *) "/usr/local/Cellar/git", 21},
    {(void *) 18, (u8 *) "/usr/local/Cellar/git-lfs", 25},
    {(void *) 19, (u8 *) "/usr/local/Cellar/go", 20},
    {(void *) 20, (u8 *) "/usr/local/Cellar/libyaml", 25},
    {(void *) 21, (u8 *) "/usr/local/Cellar/openssl", 25},
    {(void *) 22, (u8 *) "/usr/local/Cellar/pcre2", 23},
    {(void *) 23, (u8 *) "/usr/local/Cellar/pkg-config", 28},
    {(void *) 24, (u8 *) "/usr/local/Cellar/readline", 26},
    {(void *) 25, (u8 *) "/usr/local/Cellar/ruby", 22},
    {(void *) 26, (u8 *) "/usr/local/etc/bash_completion.d", 32},
    {(void *) 27, (u8 *) "/usr/local/etc/gitconfig", 24},
    {(void *) 28, (u8 *) "/usr/local/etc/openssl", 22},
    {(void *) 29, (u8 *) "/usr/local/Homebrew/.editorconfig", 33},
    {(void *) 30, (u8 *) "/usr/local/Homebrew/.git", 24},
    {(void *) 31, (u8 *) "/usr/local/Homebrew/.github", 27},
    {(void *) 32, (u8 *) "/usr/local/Homebrew/.gitignore", 30},
    {(void *) 33, (u8 *) "/usr/local/Homebrew/.yardopts", 29},
    {(void *) 34, (u8 *) "/usr/local/Homebrew/azure-pipelines.yml", 39},
    {(void *) 35, (u8 *) "/usr/local/Homebrew/bin", 23},
    {(void *) 36, (u8 *) "/usr/local/Homebrew/CHANGELOG.md", 32},
    {(void *) 37, (u8 *) "/usr/local/Homebrew/CODE_OF_CONDUCT.md", 38},
    {(void *) 38, (u8 *) "/usr/local/Homebrew/completions", 31},
    {(void *) 39, (u8 *) "/usr/local/Homebrew/CONTRIBUTING.md", 35},
    {(void *) 40, (u8 *) "/usr/local/Homebrew/Dockerfile", 30},
    {(void *) 41, (u8 *) "/usr/local/Homebrew/Dockerfile.test.yml", 39},
    {(void *) 42, (u8 *) "/usr/local/Homebrew/docs", 24},
    {(void *) 43, (u8 *) "/usr/local/Homebrew/Library", 27},
    {(void *) 44, (u8 *) "/usr/local/Homebrew/LICENSE.txt", 31},
    {(void *) 45, (u8 *) "/usr/local/Homebrew/manpages", 28},
    {(void *) 46, (u8 *) "/usr/local/Homebrew/README.md", 29},
    {(void *) 47, (u8 *) "/usr/local/include/pcre2.h", 26},
    {(void *) 48, (u8 *) "/usr/local/include/pcre2posix.h", 31},
    {(void *) 49, (u8 *) "/usr/local/include/yaml.h", 25},
    {(void *) 50, (u8 *) "/usr/local/lib/libpcre2-16.0.dylib", 34},
    {(void *) 51, (u8 *) "/usr/local/lib/libpcre2-16.a", 28},
    {(void *) 52, (u8 *) "/usr/local/lib/libpcre2-16.dylib", 32},
    {(void *) 53, (u8 *) "/usr/local/lib/libpcre2-32.0.dylib", 34},
    {(void *) 54, (u8 *) "/usr/local/lib/libpcre2-32.a", 28},
    {(void *) 55, (u8 *) "/usr/local/lib/libpcre2-32.dylib", 32},
    {(void *) 56, (u8 *) "/usr/local/lib/libpcre2-8.0.dylib", 33},
    {(void *) 57, (u8 *) "/usr/local/lib/libpcre2-8.a", 27},
    {(void *) 58, (u8 *) "/usr/local/lib/libpcre2-8.dylib", 31},
    {(void *) 59, (u8 *) "/usr/local/lib/libpcre2-posix.2.dylib", 37},
    {(void *) 60, (u8 *) "/usr/local/lib/libpcre2-posix.a", 31},
    {(void *) 61, (u8 *) "/usr/local/lib/libpcre2-posix.dylib", 35},
    {(void *) 62, (u8 *) "/usr/local/lib/libyaml-0.2.dylib", 32},
    {(void *) 63, (u8 *) "/usr/local/lib/libyaml.a", 24},
    {(void *) 64, (u8 *) "/usr/local/lib/libyaml.dylib", 28},
    {(void *) 65, (u8 *) "/usr/local/lib/pkgconfig", 24},
    {(void *) 66, (u8 *) "/usr/local/lib/ruby", 19},
    {(void *) 67, (u8 *) "/usr/local/opt/gettext", 22},
    {(void *) 68, (u8 *) "/usr/local/opt/git", 18},
    {(void *) 69, (u8 *) "/usr/local/opt/git-lfs", 22},
    {(void *) 70, (u8 *) "/usr/local/opt/go", 17},
    {(void *) 71, (u8 *) "/usr/local/opt/go@1.11", 22},
    {(void *) 72, (u8 *) "/usr/local/opt/golang", 21},
    {(void *) 73, (u8 *) "/usr/local/opt/google-go", 24},
    {(void *) 74, (u8 *) "/usr/local/opt/libyaml", 22},
    {(void *) 75, (u8 *) "/usr/local/opt/openssl", 22},
    {(void *) 76, (u8 *) "/usr/local/opt/openssl@1.0", 26},
    {(void *) 77, (u8 *) "/usr/local/opt/pcre2", 20},
    {(void *) 78, (u8 *) "/usr/local/opt/pkg-config", 25},
    {(void *) 79, (u8 *) "/usr/local/opt/pkgconfig", 24},
    {(void *) 80, (u8 *) "/usr/local/opt/readline", 23},
    {(void *) 81, (u8 *) "/usr/local/opt/ruby", 19},
    {(void *) 82, (u8 *) "/usr/local/opt/ruby@2.6", 23},
    {(void *) 83, (u8 *) "/usr/local/share/aclocal", 24},
    {(void *) 84, (u8 *) "/usr/local/share/doc", 20},
    {(void *) 85, (u8 *) "/usr/local/share/emacs", 22},
    {(void *) 86, (u8 *) "/usr/local/share/git-core", 25},
    {(void *) 87, (u8 *) "/usr/local/share/git-gui", 24},
    {(void *) 88, (u8 *) "/usr/local/share/gitk", 21},
    {(void *) 89, (u8 *) "/usr/local/share/gitweb", 23},
    {(void *) 90, (u8 *) "/usr/local/share/locale", 23},
    {(void *) 91, (u8 *) "/usr/local/share/man", 20},
    {(void *) 92, (u8 *) "/usr/local/share/perl5", 22},
    {(void *) 93, (u8 *) "/usr/local/share/zsh", 20},
    {(void *) 94, (u8 *) "/usr/local/var/homebrew", 23},
};

static void test8(void)
{
    rax *rax = raxNew();
    ASSERT_NONNULL(rax);
    ssize_t sz;
    void *data;
    int ok;
    size_t i;
    struct kv *kvp;

    char *usr_local = "/usr/local/";

    for (i = 0; i < ARRAY_SIZE(test8_data); i++) {
        kvp = &test8_data[i];
        ok = raxInsert(rax, kvp->key, kvp->len, kvp->data, NULL);
        ASSERT(ok == 1);
    }

    for (i = 0; i <= strlen(usr_local); i++) {
        sz = HEX_MAGIC;
        data = raxLongestPrefixMatch(rax, (u8 *) usr_local, i, &sz);
        ASSERT(data == raxNotFound);
        ASSERT(sz == -1);
    }

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/g", 16, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/go", 17, &sz);
    ASSERT(data == (void *) 9);
    ASSERT(sz == 17);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/godo", 19, &sz);
    ASSERT(data == (void *) 9);
    ASSERT(sz == 17);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/godoc", 20, &sz);
    ASSERT(data == (void *) 10);
    ASSERT(sz == 20);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/godoc!!", 22, &sz);
    ASSERT(data == (void *) 10);
    ASSERT(sz == 20);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/gofm", 19, &sz);
    ASSERT(data == (void *) 9);
    ASSERT(sz == 17);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/bin/gofmt", 20, &sz);
    ASSERT(data == (void *) 11);
    ASSERT(sz == 20);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/share/git", 20, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/share/gitk", 21, &sz);
    ASSERT(data == (void *) 88);
    ASSERT(sz == 21);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/share/gitk", 22, &sz);
    ASSERT(data == (void *) 88);
    ASSERT(sz == 21);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/opt/ruby", 18, &sz);
    ASSERT(data == raxNotFound);
    ASSERT(sz == -1);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/opt/ruby", 19, &sz);
    ASSERT(data == (void *) 81);
    ASSERT(sz == 19);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/opt/ruby@2.6", 22, &sz);
    ASSERT(data == (void *) 81);
    ASSERT(sz == 19);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/opt/ruby@2.6", 23, &sz);
    ASSERT(data == (void *) 82);
    ASSERT(sz == 23);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/opt/ruby@2.6foobar", 29, &sz);
    ASSERT(data == (void *) 82);
    ASSERT(sz == 23);

    sz = HEX_MAGIC;
    data = raxLongestPrefixMatch(rax, (u8 *) "/usr/local/lib/libpcre2-", 24, &sz);
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
    test6();
    test7();
    test8();
    LOG("Built: %s %s\nPass!", __DATE__, __TIME__);
    return 0;
}

