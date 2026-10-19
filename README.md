# TOY-ISA Simulator

A simple simulator for the TOY instruction set architecture (ISA).

## Build & Run

### Build

Build the project in release mode:

```sh
make all
```

### Run

To verify the simulator's functionality, run the Fibonacci example:

1. Assemble the program using `rb.asm`:

   ```sh
   ruby rb.asm
   ```

2. Run the simulator:

   ```sh
   printf '100\n' | ./build/interpreter -i tests/bin/fibonacci.bin
   ```

## Tests

To build and run tests for all ISA instructions:

```sh
make tests
```