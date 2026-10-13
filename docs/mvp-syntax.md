# Provisional MVP Syntax

This document describes implementation-only syntax so the runnable MVP can be
tested. These choices are **not approved language design** and do not change
the open questions in the language specification or decision log.

```babu
 astiram name = "Babu";
 astiram count = 2 + 3 * 4;
 count = count + 1;
chupi(name);
chupi(count / 2);

 kriya add(left: integer, right: integer) -> integer {
   astiram result = left + right;
   pampu result;
}
 chupi(add(2, 3));
 astiram greeting = "Namaste Hyderabad";
 chupi(greeting);

{
    chupi("నమస్కారం");
}
```

For this MVP only:

- `chupi(expression);` prints one value and reuses the existing example's
 output spelling. Comma-separated values are displayed consecutively, which
 supports output such as `chupi(a, " + ", b, " = ", c);`.
- `astiram name = expression;` declares an inferred mutable variable.
  Reassignment uses `name = expression;` and must keep the inferred type.
- `stiram name = expression;` declares an inferred constant that cannot be
  reassigned.
- `kriya name(parameter: type) -> type { ... }` declares a function. Parameter
  types are required; supported type names are `integer`, `float`, and
  `string`. Return type inference is supported for nonrecursive functions.
  The `kriya` keyword is approved; parameter annotations, colon syntax, and
  arrow notation remain provisional implementation choices.
- `pampu expression;` returns a value from a function. The `pampu` keyword is
  approved; the return statement form remains provisional.
  from the `pampu expression;`. A function must return a value.
- The return type annotation is optional for nonrecursive functions. The
  interpreter checks the function body at its declaration and infers the
  return type from its `return expression;`. A function must return a value.
  Recursive functions require an explicit return type so recursive calls can
  be checked. Functions must be declared before use; forward declarations and
  nested function declarations are not supported.
- Calls use `name(argument, ... )` as expressions. Argument count and types
  are checked at the call site. Each call has a local environment initialized
  from the variables visible at the function declaration; parameter and local
  assignments do not modify the caller's environment.
- Recursive calls are type-checked and supported by the evaluator. Since this
  MVP has no conditional or loop statements, it cannot express a recursive
  base case; invoking a recursive function will not terminate.
- Values are signed 64-bit integers, floating-point numbers, or UTF-8 strings.
  Integer `+`, `-`, and `*` remain integers; `/` returns a floating-point
  number; `%` requires integers. Mixed numeric operations return a float.
- `+` concatenates two strings. Parentheses and normal arithmetic precedence
  are supported. Integer overflow, division by zero, undefined variables,
  and type mismatches are errors.
- `{ ... }` groups statements. Ordinary statements, including `return`, end
- `{ ... }` groups statements. Ordinary statements, including `pampu`, end
  with semicolons; a function declaration ends with its closing brace and no
  semicolon. Blocks do not introduce scope or imply control flow. Conditionals,
  and loops are not implemented.
- The earlier MVP spellings `fun`, `return`, and assignment-on-first-use remain
  supported only for compatibility with existing examples and tests. They are
  provisional legacy forms, not approved keywords or syntax recommendations.
- Ordinary statements require semicolons. Strings preserve UTF-8 bytes and
  support `\n`, `\r`, `\t`, `\\`, and `\"` escapes.

This is an interpreter MVP, not a native-code compiler. Syntax, numeric rules,
 This is an interpreter MVP, not a native-code compiler. Syntax, numeric rules,
 string behavior, function signatures, and the output form remain subject to
language-design review. Function bodies are checked when their declarations
are processed; argument checks occur when calls are evaluated. There is no
separate compile-only phase.