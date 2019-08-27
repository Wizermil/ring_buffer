#pragma once

#define RING_BUFFER_HIDDEN [[gnu::visibility("hidden")]]
#define RING_BUFFER_INTERNAL_LINKAGE [[clang::internal_linkage]]

#define RING_BUFFER_HIDE_FROM_ABI RING_BUFFER_HIDDEN RING_BUFFER_INTERNAL_LINKAGE

#define RING_BUFFER_LIKELY(exp) (__builtin_expect((exp), true))
#define RING_BUFFER_UNLIKELY(exp) (__builtin_expect((exp), false))
