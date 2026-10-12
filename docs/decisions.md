# Decisions

This document distinguishes between established project decisions, proposed working principles, and unresolved design questions.

## Established decisions

### D-001
Language Name: Babu

### D-002
Tagline: Telugu vaari computer bhasha

### D-003
Implementation Language: C++

### D-004
Development Environment: GitHub Codespaces

### D-005
Project Type: Programming Language

### D-006
Execution Strategy: Compiler First

### D-007
Syntax Direction: Follow Python as closely as practical for readability and simplicity, with code blocks delimited by curly braces (`{}`) rather than indentation and ordinary statements terminated by semicolons (`;`).

### D-008
Type System Direction: Static typing with compile-time type inference.

### D-009
Variable Mutability: Variables are mutable by default.

### D-010
Constants: Constants will be supported from the first version and use binding immutability: a constant cannot be reassigned after initialization, but an object or collection referenced by it may still be mutable. Deep immutability is not part of this decision and remains a possible future design topic.

### D-011
Uninitialized Variables: An uninitialized variable must have an explicit type or another reliable way for the compiler to determine its type. Exact syntax and the criteria for reliable type determination remain unresolved.

## Proposed primitive-type baseline (not approved)

The following recommendations are a proposed v1 direction for review, not established decisions. See the [language specification](language-specification.md) for trade-offs and unresolved details.

- Initial numeric categories: integer and floating-point; recommend arbitrary-precision integers by default and one standard floating-point type initially.
- Include built-in text and boolean types; text supports Unicode, including Telugu.
- Do not include a separate character type initially; individual characters are represented as strings.
- Recommend Unicode code-point-based string indexing initially.
- Defer optional types until the core type system is stable.
- Static typing with compile-time type inference remains an approved decision recorded in D-008; this proposal does not change its status.

## Approved language philosophy

The following principles are now approved for Babu:

1. Balanced design: approachable for beginners while capable of supporting serious software development.
2. Predictability over convenience: consistent, well-defined behavior without surprising implicit behavior.
3. Simplicity with purpose: every feature must justify its complexity.
4. Practical safety: prevent common programming mistakes without unnecessary restrictions.
5. Progressive learning: support incremental learning without requiring beginners to understand every feature.
6. Professional capability: design toward maintainable, testable, and deployable software.
7. Independent identity: develop a coherent language rather than superficially imitating existing languages.
8. Telugu-inspired design: Telugu identity must influence the language's philosophy and developer experience. Its specific expression remains undecided.
9. Long-term consistency: evaluate new features against established principles.

## Approved design boundaries

The following boundaries are also approved for Babu:

1. One coherent language for beginners and professionals, with consistent underlying rules.
2. Readability matters: programs should be understandable without excessive punctuation or hidden behavior.
3. Explicit semantics: program behavior must follow documented language rules.
4. No feature inflation: Babu does not need every feature found in existing languages.
5. Evolution with compatibility in mind: future changes should consider existing programs and migration costs.
6. Implementation-independent design: specify language behavior independently of compiler internals wherever practical.
7. Telugu identity without premature assumptions: develop a deliberate Telugu-inspired identity without prematurely deciding how it must appear in syntax or vocabulary.

## Remaining syntax and type-system questions

The foundational syntax direction and typing approach above are confirmed, but many details remain open. In particular, keywords, variable declaration syntax, type annotation syntax, detailed type rules, and full statement/block behavior have not been approved. See the [language specification](language-specification.md) for the structured list of unresolved topics.

## Pending decisions

The following areas remain unresolved and must be decided deliberately before implementation:

- Further design decisions beyond the approved language philosophy and boundaries
- Keywords and reserved words
- Primitive-type recommendations and detailed type-system rules
- Scope and name-resolution rules
- Memory model and runtime assumptions
- Compiler architecture and intermediate representation strategy
- Diagnostics and error-reporting design
- Standard library strategy
- Module and package system
- Testing and conformance strategy
- Build and release process
- Cross-platform support expectations
- Security considerations and sandboxing requirements
- Editor, formatter, and language-server tooling strategy

## Important note

The project has a confirmed Compiler First strategy, but the detailed compiler architecture, backend target, and runtime model remain undecided. These questions should be addressed only after language philosophy and architecture constraints are documented and approved.