# Babu — Telugu vaari computer bhasha

Babu is an experimental, Telugu-inspired programming language designed to explore a serious language identity grounded in readability, clarity, and practical developer experience. The project is intentionally not a direct Telugu translation of Python, Java, C++, or any other language. Instead, it aims to develop a coherent language philosophy and direction that can serve both beginners and professional developers.

## Current status

Babu is experimental. The repository includes a small C++17 interpreter MVP
for a deliberately provisional subset of the language; it is not production-ready.

Established direction:
- Language name: Babu
- Implementation language: C++
- Development environment: GitHub Codespaces
- Execution strategy: Compiler First

The exact MVP syntax is documented separately in
[docs/mvp-syntax.md](docs/mvp-syntax.md). It is not approved language design and
does not settle the open syntax questions in the specification.

## Build and run the MVP

The only build prerequisite is a C++17 compiler such as `g++` and GNU Make,
both available in GitHub Codespaces. From the repository root:

```sh
make
./babu examples/greeting.babu
./babu examples/addition.babu
make test
```

`make` builds the `babu` executable. It accepts one `.babu` source file and
 prints output to the terminal. The greeting command prints `Namaste
 Hyderabad`; the addition command prints `7 + 5 = 12`. `make test` runs the
 complete regression and demo suite.

The MVP supports string and numeric literals, inferred mutable variables,
 The MVP supports string and numeric literals, inferred mutable declarations
 with `astiram`, constants with `stiram`, `kriya` functions and `pampu` returns,
 integer and floating-point arithmetic, string concatenation, and `chupi(...)`
 output. Statements use semicolons and blocks use braces. It preserves UTF-8
 string contents, including Telugu. Static type consistency is checked for
 variable reassignment and expression operations, with file/line/column
 diagnostics. Function parameter and return syntax and multi-value output remain
 provisional; details are in [docs/mvp-syntax.md](docs/mvp-syntax.md).

 It does not implement explicit type annotations for variables, conditions,
 loops, imports, or native-code compilation. The interpreter evaluates the
 supported syntax directly. Older provisional `fun`, `return`, and implicit
 first-assignment forms remain supported for existing programs.

The [language specification](docs/language-specification.md) records confirmed
decisions separately from provisional recommendations and unresolved questions;
exact syntax and keywords remain under maintainer control. See the
[roadmap](docs/roadmap.md) for the broader project direction.

## Purpose and vision

Babu is meant to be a programming language with its own identity, shaped by Telugu cultural and linguistic sensibilities without sacrificing technical seriousness. The project values:
- approachable design for learners and new developers
- predictable language behavior
- practical developer ergonomics
- maintainable, scalable language evolution
- honest, helpful diagnostics and documentation

## Intended audience

Babu targets a broad range of developers, including:
- beginners learning programming concepts
- developers seeking a readable, expressive language
- experienced engineers evaluating a language with a clear design philosophy

## Design position

Babu’s approved language philosophy is:
1. Balanced design: approachable for beginners while capable of supporting serious software development.
2. Predictability over convenience: consistent, well-defined behavior without surprising implicit behavior.
3. Simplicity with purpose: every feature must justify its complexity.
4. Practical safety: prevent common programming mistakes without unnecessary restrictions.
5. Progressive learning: support incremental learning without requiring beginners to understand every feature.
6. Professional capability: design toward maintainable, testable, and deployable software.
7. Independent identity: develop a coherent language rather than superficially imitating existing languages.
8. Telugu-inspired design: Telugu identity must influence the language's philosophy and developer experience. Its specific expression remains undecided.
9. Long-term consistency: evaluate new features against established principles.

The approved design boundaries are:
1. One coherent language for beginners and professionals, with consistent underlying rules.
2. Readability matters: programs should be understandable without excessive punctuation or hidden behavior.
3. Explicit semantics: program behavior must follow documented language rules.
4. No feature inflation: Babu does not need every feature found in existing languages.
5. Evolution with compatibility in mind: future changes should consider existing programs and migration costs.
6. Implementation-independent design: specify language behavior independently of compiler internals wherever practical.
7. Telugu identity without premature assumptions: develop a deliberate Telugu-inspired identity without prematurely deciding how it must appear in syntax or vocabulary.

These principles and boundaries are now recorded as the approved design foundation for Babu. Further syntax and grammar details, keywords, detailed type-system rules, memory management, runtime architecture, and compiler backend remain unresolved.

## Repository documentation

- [docs/vision.md](docs/vision.md) — project vision, goals, and working design principles
- [docs/language-specification.md](docs/language-specification.md) — central record of confirmed language decisions and open design topics
- [docs/decisions.md](docs/decisions.md) — established decisions, proposed principles, and pending unresolved questions
- [docs/roadmap.md](docs/roadmap.md) — current design and implementation roadmap
- [docs/project-brief.md](docs/project-brief.md) — short project summary and current milestone

## Design status

The language specification and decisions document remain authoritative for
approved design. The MVP makes narrow implementation choices only; see
[docs/mvp-syntax.md](docs/mvp-syntax.md) for their provisional status and
limitations.

## Notes

- The sample program in [examples/hello.babu](examples/hello.babu) demonstrates MVP behavior and is not a normative language specification.
- Keyword design, syntax decisions, and implementation choices remain subject to explicit maintainer review.
- This project is experimental and should not be treated as production-ready software.

## License

This project is licensed under the [MIT License](LICENSE).
