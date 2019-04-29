Coding standards 
==============

# Indentation and spacing #
- All indentation should be done with spaces
- Each level of indentation should be 2 spaces (level: lines of code inside a
  set of braces)
- There should be 1 space between a conditional keyword and the parentheses for
  its clause
- Braces are always on their own line, at the same indentation level as the
  block identifier (`if`, `for`, `try`, etc.)

# Orthography #
- The project name should not be incorporated in class names.
- Class names and namespace names should be CamelCase with a leading capital.
- Method, field, and function names should use underscores as word separators,
  all lowercase.
- When necessary, prefix a parameter with 'p', don't add additional characters 
  to field names.
- Constants should be declared with a `c_` prefix, macros should be all-caps.
  Prefer const-declared constants to macros.

# Class structure #
- All methods that do not modify object state should be declared const.
- Fields should be sorted by type so that the compiler can properly optimize the
  object structure.

# Objects and pointers #
- Avoid heap-allocation as much as possible; whenever creating an object inside
  a limited scope, stack-allocate the object.
- Avoid handling or returning bare pointers as much as possible; instead, try
  to wrap pointers in `unique\_ptr<T>` and `shared\_ptr<T>` objects.
- Prefer constructors which use initialization list syntax to reduce boilerplate
  code.
- Prefer references to pointers for method and function calls.
  (i.e. `A::fn(B& pr)` over `A::fn(B *pr)`.)
- Prefer returning an object to returning a pointer to an object.
- When using a framework which returns bare pointers, attempt to wrap all such
  pointers in RAII-compatible containers as soon as possible.
- Prefer `static\_cast<T>` and `dynamic\_cast<T>` to C-style casts for bare
  pointers, use `static\_pointer\_cast<T>` and `dynamic\_pointer\_cast<T>` for
  wrapped pointer types.
- Prefer dynamic casts to static casts of pointers, in initial development.  As
  development continues, if this proves to be too slow, convert proven safe
  dynamic casts to static casts (i.e. move from run-time to compile-time.)
- Avoid `const\_cast<T>` and `const\_pointer\_cast<T>` whereever possible, to
  preserve const-correctness as much as possible.

# Exception handling #
Avoiding exceptions at the outset of development doesn't make much sense; they
are an expressive tool, and when combined with proper RAII, vastly simplify
memory-management.  The guideline is to catch exceptions that can be predicted
to happen infrequently in normal execution.  Do not throw an exception in the
case of a non-optimal result (prefer returning false to throwing an
InvalidResultException.)