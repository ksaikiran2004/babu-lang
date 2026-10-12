# Babu Language Project Brief

Tagline: Telugu vaari computer bhasha

## Current status

The project is in the Language Design phase. The repository includes the initial documentation structure and example material, but implementation work has not begun.

## Repository structure

- docs/ — project vision, decisions, roadmap, and project brief
- examples/ — language example files
- src/ — implementation directory placeholder
- tests/ — test directory placeholder
- assets/ — non-code project assets

## Implementation direction

- Implementation language: C++
- Development environment: GitHub Codespaces
- Execution strategy: Compiler First

## Project vision

Babu aims to become a serious programming language with an independent identity and a Telugu-inspired perspective. It is being designed as a legitimate language effort rather than a literal translation of existing languages or a novelty project.

## Established decisions

- Repository: babu-lang
- Language name: Babu
- Tagline: Telugu vaari computer bhasha
- Implementation language: C++
- Development environment: GitHub Codespaces
- Compiler First execution strategy

## Approved language philosophy

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

## Approved design boundaries

The approved design boundaries for Babu are:

1. One coherent language for beginners and professionals, with consistent underlying rules.
2. Readability matters: programs should be understandable without excessive punctuation or hidden behavior.
3. Explicit semantics: program behavior must follow documented language rules.
4. No feature inflation: Babu does not need every feature found in existing languages.
5. Evolution with compatibility in mind: future changes should consider existing programs and migration costs.
6. Implementation-independent design: specify language behavior independently of compiler internals wherever practical.
7. Telugu identity without premature assumptions: develop a deliberate Telugu-inspired identity without prematurely deciding how it must appear in syntax or vocabulary.

## Next milestone

The current milestone is Language Specification development. The [language specification](language-specification.md) separates confirmed decisions, provisional recommendations, and unresolved design questions. Keywords, variable declaration syntax, type annotation syntax, and other unapproved details remain open for explicit review.
