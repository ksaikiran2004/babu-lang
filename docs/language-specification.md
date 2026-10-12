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
- Variables are mutable by default.
- Constants will be supported from the first version and use binding immutability: a constant cannot be reassigned after initialization, but an object or collection referenced by it may still be mutable. Deep immutability is not part of this decision.
- An uninitialized variable must have an explicit type or another reliable way for the compiler to determine its type.

These decisions do not finalize keywords, variable declaration syntax, type annotation syntax, or other syntax not explicitly described here.

## Provisional recommendations

The following are process recommendations, not approved language behavior:

- Resolve design topics in the order listed below, recording each approved decision and the rationale in the project decision log.
- Evaluate proposals against readability, predictable behavior, simplicity, practical safety, and suitability for both learning and serious software development.
- Use small illustrative examples to compare alternatives, but label them non-normative until the relevant syntax and semantics are approved.

No recommendation in this section selects keywords or exact syntax.

## Variable semantics and constants

The following analysis separates approved decisions from recommendations that remain provisional.

### Approved decisions

- Variables are mutable by default. This supports straightforward reassignment, while making it easier for a value to change unexpectedly. Immutable-by-default variables would make change more explicit and can simplify reasoning, but may require more explicit opt-ins for ordinary updates.
- Constants will be supported from the first version and use binding immutability: a constant cannot be reassigned after initialization, but an object or collection referenced by it may still be mutable. This does not establish deep immutability.
- An uninitialized variable must have an explicit type or another reliable way for the compiler to determine its type. The meaning of “reliable” and the exact diagnostics remain open.

### Provisional recommendations

- Reassignment is permitted after a variable's type has been inferred. This supports normal state updates without requiring declaration syntax to be settled first.
- A reassigned value must remain compatible with the variable's original inferred type. This preserves static guarantees and predictable use of the variable; the exact meaning of compatibility is unresolved.
- Type inference occurs at compile time, as already confirmed. When available initializers or context do not determine a type, the recommendation is to require additional type information rather than guess a type. Whether that situation must always be resolved by an annotation, and how it is diagnosed, remain open.
- Explicit type annotations should be permitted when inference is insufficient. This is a recommendation only; the annotation form and its relationship to declaration syntax are not selected.

### Trade-offs to resolve

- **Default mutability:** Mutable-by-default behavior can feel direct and familiar, but requires care to track state changes. Immutable-by-default behavior can improve local reasoning and reduce accidental changes, but adds friction where updates are routine. The default is approved as mutable.
- **Reassignment and type compatibility:** Allowing reassignment while preserving the inferred type gives variables a stable static type. Broader compatibility rules can be expressive but may make assignments less predictable; strict identity can be easier to explain but restrictive. No conversion, subtype, or compatibility policy is established.
- **Insufficient inference information:** Inferring only from adequate compile-time information avoids arbitrary guesses. Requiring an annotation makes the programmer's intent explicit but adds typing; using surrounding context may reduce annotations but can make inference less local. No fallback type or exact failure policy is approved.
- **Constants:** Binding immutability prevents reassignment of the constant itself while allowing a referenced object's or collection's contents to remain mutable. This avoids implying recursive freezing, but leaves questions such as whether initialization must be compile-time evaluable. Deep immutability is not approved and may be considered in future design.
- **Explicit annotations:** Annotations can resolve ambiguity and communicate intent, while extensive annotation requirements can undermine the readability goal. The circumstances in which annotations are required or optional remain undecided.

### Unresolved questions requiring approval

- Is reassignment allowed after type inference, and what exactly counts as type-compatible? Are conversions or other forms of compatibility permitted?
- What qualifies as another reliable way for the compiler to determine an uninitialized variable's type? What should happen when no such information is available?
- Must a constant's initializer be compile-time evaluable? Deep immutability is not part of the approved decision and remains a possible future design topic.
- Beyond providing a type for an otherwise uninitialized variable, when may explicit type annotations be used? Their syntax remains unapproved.

## Primitive types: proposed v1 baseline

The choices in this section are the proposed design direction for review, not approved language rules. The existing confirmed decisions for static typing and compile-time type inference remain approved and are not downgraded by this proposal.

### Proposed recommendations

- Start with integer and floating-point numeric categories.
- Recommend arbitrary-precision integers as the default integer category. This avoids a fixed upper bound on integer values, at the cost of potentially greater memory use and slower arithmetic than bounded machine integers.
- Include one standard floating-point type initially. A single choice keeps the initial type system simpler, while leaving its precision, range, rounding, and exceptional-value behavior to be specified.
- Include built-in text and boolean types.
- Text supports Unicode, including Telugu. The exact encoding, normalization, equality, and text-operation rules remain to be specified.
- Do not introduce a separate character type initially; individual characters are represented as strings. This simplifies the initial type set, while string length and indexing still need clear definitions.
- Recommend Unicode code-point-based string indexing initially. Code points provide a consistent unit across Unicode text, but do not always correspond to a user-perceived character, such as a grapheme formed from multiple code points.
- Defer optional types until the core type system is stable. This limits initial type-system complexity, while leaving representation and handling of absent values for a later design stage.
- Keep compile-time type inference and static type checking as confirmed decisions; this proposed type baseline does not alter them.

### Unresolved primitive-type questions

- What integer arithmetic, overflow, division, and conversion rules should apply, including operations that combine integers and floating-point values?
- What precision, range, rounding behavior, and exceptional values should the single standard floating-point type have?
- What Unicode encoding and normalization behavior should text use, and how should equality be defined?
- How should indexing handle bounds, and what operations, if any, should work in grapheme clusters rather than code points?
- What boolean operations and conversions are supported?
- When will the core type system be considered stable enough to revisit optional types, and how should absent values be represented then?

## Unresolved design topics

The topics below remain open. Their order is a suggested design sequence, not an approval of any particular design.

1. **Variables and constants** — see the analysis and approval questions above; declaration and initialization syntax, scope, and name resolution also remain unresolved. Do not treat any declaration or type annotation form as selected.
2. **Primitive types** — see the proposed v1 baseline and unresolved questions above; these recommendations await explicit approval.
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