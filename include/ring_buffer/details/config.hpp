#pragma once

#define DEQUE_LIKELY(exp) (__builtin_expect((exp), true))
#define DEQUE_UNLIKELY(exp) (__builtin_expect((exp), false))
