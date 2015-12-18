#if defined(__clang__) || defined(__GNUC__)
#define FINLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define FINLINE __forceinline
#else
#pragma message "Can't estabish compiler's forceinline directive (if any)"
#define FINLINE inline
#endif
