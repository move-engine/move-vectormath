#pragma once
#include <rtm/macros.h>

#define MVM_FORCE_INLINE RTM_FORCE_INLINE

#if __has_cpp_attribute(nodiscard)
#define MVM_NODISCARD [[nodiscard]]
#elif defined(__GNUC__) || defined(__clang__)
#define MVM_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
#define MVM_NODISCARD _Check_return_
#elif
#define MVM_NODISCARD
#endif

#define MVM_INLINE MVM_FORCE_INLINE
#define MVM_INLINE_NODISCARD MVM_NODISCARD MVM_FORCE_INLINE