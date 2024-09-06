#pragma once

#if defined(_MSC_VER)
#define MVM_IS_MSVC
#endif

#if defined(__GNUC__)
#define MVM_IS_GCC
#endif

#if defined(__clang__)
#define MVM_IS_CLANG
#endif

#if defined(MVM_IS_MSVC)
#define MVM_FORCE_INLINE __forceinline
#elif defined(MVM_IS_GCC) || defined(MVM_IS_CLANG)
#define MVM_FORCE_INLINE __attribute__((always_inline)) inline
#else
#define MVM_FORCE_INLINE inline
#endif

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