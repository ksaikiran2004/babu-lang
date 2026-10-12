# Provisional MVP Syntax

This document describes implementation-only syntax so the runnable MVP can be
tested. These choices are **not approved language design** and do not change
the open questions in the language specification or decision log.

```babu
name = "Babu";
count = 2 + 3 * 4;
count = count + 1;
chupi(name);
chupi(count / 2);

{
    chupi("నమస్కారం");
}
```

For this MVP only:

- `chupi(expression);` prints one value and reuses the existing example's
  output spelling; it is not a finalized keyword or standard-library API.
- `name = expression;` creates a mutable variable on first assignment and
  permits later assignments only when the inferred type is unchanged.
- Values are signed 64-bit integers, floating-point numbers, or UTF-8 strings.
  Integer `+`, `-`, and `*` remain integers; `/` returns a floating-point
  number; `%` requires integers. Mixed numeric operations return a float.
- `+` concatenates two strings. Parentheses and normal arithmetic precedence
  are supported. Integer overflow, division by zero, undefined variables,
  and type mismatches are errors.
- `{ ... }` groups statements. Blocks do not introduce scope or imply control
  flow; conditionals, loops, functions, constants, and declarations are not
  implemented.
- Ordinary statements require semicolons. Strings preserve UTF-8 bytes and
  support `\n`, `\r`, `\t`, `\\`, and `\"` escapes.

This is an interpreter MVP, not a native-code compiler. Syntax, numeric rules,
string behavior, and the output form remain subject to language-design review.