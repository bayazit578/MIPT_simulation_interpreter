# Repository Guidelines

## Project Structure & Module Organization

This repository implements a small C++20 CPU instruction interpreter.
- `include/isa.hpp` defines word types, opcodes, instruction fields, and machine constants.
- `include/cpu.hpp` and `src/cpu.cpp` implement CPU state, fetching, decoding, and execution.
- `include/memory.hpp` contains the memory implementation and bounds checks.
- `include/interpreter.hpp` and `src/interpreter.cpp` provide program loading and interpreter orchestration.
- `src/main.cpp` currently runs a hard-coded instruction smoke test. There is no program-file command-line interface or dedicated test directory.

## Build, Test, and Development Commands

Use a C++20-capable compiler. From the repository root:

```sh
g++ -std=c++20 -Wall -Wextra -g -Iinclude src/main.cpp src/cpu.cpp src/interpreter.cpp -o interpreter
./interpreter
```

The first command builds the current sources; the second runs the smoke test. Add `-DDEBUG` to enable code guarded by `IF_DEBUG`. For runtime checks, add `-fsanitize=address,undefined -fno-omit-frame-pointer` when compiling.

The Makefile contains configuration variables but no build recipes, and its source list references removed files. Update it before relying on `make`. The local `compile_commands.json` also contains stale paths.

## Coding Style & Naming Conventions

Match the existing two-space indentation and same-line opening braces for functions. Use `PascalCase` for types (`Cpu`, `Instruction`), `snake_case` for functions and variables (`fetch_instr`), and `k`-prefixed constants (`kWordSize`). Keep headers in `include/` with `.hpp` extensions and `#pragma once`; implementations belong in `src/`. Use fixed-width integer types for machine data. No formatter or linter configuration is present.

## Testing Guidelines

There is no test framework or coverage threshold. Build and run the smoke test after changes; successful execution alone does not verify instruction semantics. For instruction changes, check decoded fields, register results, memory effects, and program-counter updates. Include boundary cases for signed immediates, shifts, and memory access. Place future regression tests in `tests/`, with descriptive names such as `cpu_decode_test.cpp`, and document their execution command.

## Commit & Pull Request Guidelines

History uses short subjects with tags such as `[ADDED]:` and `[MINOR]:`. Follow that pattern and describe the specific change. Keep commits focused. Pull requests should explain behavior changes, affected instructions, and validation commands and results. Link relevant issues when available. Avoid committing generated binaries, precompiled headers, or debugger history, and preserve unrelated working-tree edits.
