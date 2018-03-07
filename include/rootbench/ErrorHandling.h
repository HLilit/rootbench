///\file Error handling.

namespace RB {
  /// This function calls abort(), and prints the optional message to stderr.
  /// Use the rb_unreachable macro (that adds location info), instead of
  /// calling this function directly.
  ///
  void
  rb_unreachable_internal(const char *msg = nullptr, const char *file = nullptr,
                          unsigned line = 0);
}

/// RB_BUILTIN_UNREACHABLE - On compilers which support it, expands to an
/// expression which states that it is undefined behavior for the compiler to
/// reach this point. Otherwise is not defined.
///
#if __has_builtin(__builtin_unreachable)
# define RB_BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
# define RB_BUILTIN_UNREACHABLE __assume(false)
#endif

/// Marks that the current location is not supposed to be reachable.
/// In !NDEBUG builds, prints the message and location info to stderr.
/// In NDEBUG builds, becomes an optimizer hint that the current location
/// is not supposed to be reachable. On compilers that don't support such hints,
/// prints a reduced message instead.
///
/// Use this instead of assert(0). It conveys intent more clearly and allows
/// compilers to omit some unnecessary code.
///
#ifndef NDEBUG
#define rb_unreachable(msg)                                   \
  ::rb::rb_unreachable_internal(msg, __FILE__, __LINE__)
#elif defined(RB_BUILTIN_UNREACHABLE)
#define rb_unreachable(msg) RB_BUILTIN_UNREACHABLE
#else
#define rb_unreachable(msg) ::rb::rb_unreachable_internal()
#endif
