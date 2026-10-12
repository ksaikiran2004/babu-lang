# Babu — Telugu vaari computer bhasha

Babu is an experimental, Telugu-inspired programming language designed to explore a serious language identity grounded in readability, clarity, and practical developer experience. The project is intentionally not a direct Telugu translation of Python, Java, C++, or any other language. Instead, it aims to develop a coherent language philosophy and direction that can serve both beginners and professional developers.

## Current status

Babu is in the Language Design stage. The repository contains initial project documentation and a small example, but the language is not yet implemented and is not production-ready.

Established direction:
- Language name: Babu
- Implementation language: C++
- Development environment: GitHub Codespaces
- Execution strategy: Compiler First
- Current phase: Language Design

This project is experimental and intended to evolve through design review before implementation begins.

The current milestone is **Language Specification development**. The [language specification](docs/language-specification.md) records confirmed decisions separately from provisional recommendations and unresolved questions; exact syntax and keywords remain under maintainer control. See the [roadmap](docs/roadmap.md) for project milestones.

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

## Current implementation status

Babu does not yet include:
- a lexer or parser
- a compiler or interpreter
- a complete syntax or grammar
- finalized keywords or detailed type-system rules
- a runtime, backend, or standard library specification

The repository is intentionally documentation-first at this stage.

## Notes

- The sample program in [examples/hello.babu](examples/hello.babu) is an example artifact and not a normative language specification.
- Keyword design, syntax decisions, and implementation choices remain subject to explicit maintainer review.
- This project is experimental and should not be treated as production-ready software.

## License

This project is licensed under the [MIT License](LICENSE).
