# Babu Language Specification

This document is the central record of Babu's language design. It distinguishes approved decisions from provisional guidance and questions that still require maintainer review. It is a living design document, not a compiler implementation plan.

## Status

Babu is in the Language Design phase. Specification development is the current milestone. Only items in **Confirmed decisions** are approved. Provisional recommendations are nonbinding, and unresolved questions must remain open until explicitly decided.

## Confirmed decisions

- Language name: **Babu**
- Tagline: **Telugu vaari computer bhasha**
- Implementation language: **C++**
- Syntax should follow Python as closely as practical for readability and simplicity. This does not mean copying Python wholesale; the confirmed block and statement rules below take precedence wherever they differ.
- Code blocks use curly braces (`{}`), not indentation.
- Ordinary statements end with semicolons (`;`), following C/C++/Java conventions.
- The type system is statically typed, with compile-time type inference.

These decisions do not finalize keywords, variable declaration syntax, type annotation syntax, or other syntax not explicitly described here.

## Provisional recommendations

The following are process recommendations, not approved language behavior:

- Resolve design topics in the order listed below, recording each approved decision and the rationale in the project decision log.
- Evaluate proposals against readability, predictable behavior, simplicity, practical safety, and suitability for both learning and serious software development.
- Use small illustrative examples to compare alternatives, but label them non-normative until the relevant syntax and semantics are approved.

No recommendation in this section selects keywords or exact syntax.

## Unresolved design topics

The topics below remain open. Their order is a suggested design sequence, not an approval of any particular design.

1. **Variables and constants** — declaration and initialization syntax, mutability, scope, and name resolution. Do not treat any declaration or type annotation form as selected.
2. **Primitive types** — the type set, numeric behavior, text and boolean representation, conversions, and the boundaries of compile-time inference.
3. **Functions** — definition and call syntax, parameters, return behavior, overloading, and function values.
4. **Conditions** — conditional forms, truth evaluation, and branch behavior.
5. **Loops** — available iteration and repetition forms, loop control, and iteration semantics.
6. **Operators** — operator set, precedence, associativity, and overloadability.
7. **Collections** — collection kinds, literal and access syntax, mutability, and iteration behavior.
8. **Classes** — whether classes are included, and any object, member, inheritance, or interface model.
9. **Modules** — file/module boundaries, imports, visibility, and package relationships.
10. **Error handling** — failure representation, propagation, recovery, and diagnostics behavior.
11. **Statement and block rules** — the interaction of braces and semicolons with declarations, control-flow forms, empty statements, and any statement that is not ordinary. The confirmed decisions establish curly-brace blocks and semicolons for ordinary statements, but do not settle every boundary case.

## Scope and implementation boundary

This specification describes intended language behavior independently of compiler internals wherever practical. It does not authorize or define a lexer, parser, compiler, interpreter, runtime, backend, or standard library implementation. Such implementation work remains outside the current milestone.

## Change control

An item becomes confirmed only through explicit maintainer approval and should then be recorded in [decisions.md](decisions.md) and reflected here. Until then, examples and recommendations are exploratory and non-normative.