# Roadmap

This roadmap reflects the current stage of the project and should be read as a design progression, not as a commitment to implementation before the necessary decisions are approved.

## Phase 1 - Language Design

- Maintain and apply the approved language philosophy and design boundaries
- Clarify intended audience and project goals
- Define high-level language constraints and identity
- Resolve open architecture questions before implementation
- Determine the design process for syntax, semantics, and diagnostics without locking in premature choices
- Keep undecided syntax, keywords, type-system details, memory management, runtime assumptions, and backend targets open until explicit approval

## Phase 2 - Language Specification

- Maintain the central specification of confirmed decisions, provisional recommendations, and unresolved questions
- Resolve language behavior and syntax topic by topic through explicit review
- Identify open runtime and type-system questions without assuming implementation details
- Clarify compiler-first architecture assumptions

## Phase 3 - Core Compiler Infrastructure

- Front-end architecture planning
- AST and semantic analysis design
- Intermediate representation strategy
- Error handling and diagnostics framework
- Compiler tooling and validation approach

## Phase 4 - Language Features

- Variables
- Strings
- Numbers
- Expressions
- Conditions
- Loops
- Functions

## Phase 5 - Advanced Features

- Modules
- Classes
- Standard library
- Package and distribution strategy

## Phase 6 - Tooling and Deployment

- Documentation pipeline
- Testing and conformance strategy
- Package manager planning
- IDE support
- Language server support
- Release and distribution process

## Phase 7 - Future Expansion

- Additional backends
- Optimization work
- Broader ecosystem tooling
- Community contribution processes

## Current milestone

The current milestone is **Language Specification development**, within the Language Design phase. The [language specification](language-specification.md) records the approved foundational decisions and organizes remaining design work. Keywords, declaration syntax, type annotation syntax, and other unapproved details remain open for explicit review. This milestone does not authorize compiler, interpreter, lexer, parser, or runtime implementation.