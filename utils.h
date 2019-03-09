/*
 * Created 190309 lynnl
 */

#ifndef RAX_LPM_UTILS_H
#define RAX_LPM_UTILS_H

#include <assert.h>

#define ASSERT              assert
#define ASSERT_NONNULL(p)   ASSERT(p != NULL)

#ifdef __clang__
#define __nonnull       _Nonnull
#define __nullable      _Nullable
#else
#define __nonnull       __attribute__ ((nonnull))
#define __nullable      /* Unavailable */
#endif

#endif

