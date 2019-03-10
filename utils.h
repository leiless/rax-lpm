/*
 * Created 190309 lynnl
 */

#ifndef RAX_LPM_UTILS_H
#define RAX_LPM_UTILS_H

#include <assert.h>

#define ASSERT              assert
#define ASSERT_NONNULL(p)   ASSERT(p != NULL)

#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(*a))

#ifdef __clang__
#define __nonnull       _Nonnull
#define __nullable      _Nullable
#else
#ifdef __GNUC__
#define __nonnull       __attribute__ ((nonnull))
#else
#define __nonnull       /* Unavailable */
#endif  /* __GNUC__ */
#define __nullable      /* Unavailable */
#endif  /* __clang__ */

#endif

