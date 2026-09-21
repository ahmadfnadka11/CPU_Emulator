# CPU Emulator

A custom 32-bit CPU built from scratch in C — a hand-designed instruction set architecture, a fetch-decode-execute emulator, and an assembler that translates human-readable assembly into the emulator's binary machine code.

## Overview

This project implements a small but complete computer from the ground up: a 32-bit instruction encoding, a virtual CPU that executes those instructions, and a text-to-binary assembler so programs can be written in readable assembly rather than hand-encoded machine words. It was built to explore how software and hardware meet — how a fixed set of bits becomes a decision a processor acts on.

**Highlights:**
- 23-instruction custom ISA covering arithmetic, logic, memory access, branching, and function calls (`JAL`/`JR`)
- Two independent C programs — `assembler` and `emulator` — connected only by a shared binary instruction format, mirroring how a real toolchain separates compilation from execution
- Verified correctness with hand-written test programs, including a loop (sum 1–10) and a function call/return sequence

## Architecture

**Registers:** 16 general-purpose registers (`R0`–`R15`), each 32 bits wide, plus a program counter (`PC`).

**Memory:** 1024 words of memory-mapped instruction storage, addressed by the `PC`.

**Instruction formats:** every instruction is a single 32-bit word, encoded in one of two layouts:

| Format | Layout | Used for |
|---|---|---|
| **A** (register-register) | `opcode(5)` \| `rd(4)` \| `rs1(4)` \| `rs2(4)` \| *unused(15)* | Instructions operating purely on registers (e.g. `ADD`, `SUB`, `MOV`) |
| **B** (register + immediate) | `opcode(5)` \| `rd/rs(4)` \| `rs1(4)` \| `imm(19)` | Instructions that need a literal value (e.g. `LOADI`, branches, jumps) |

The assembler tags every instruction with its exact operand shape (how many operands it takes, and whether each one is a register or an immediate), so the encoder never has to guess how to interpret a line of assembly.

## Instruction Set

| Opcode | Mnemonic | Format | Operands | Description |
|---|---|---|---|---|
| 0 | `HALT` | A | — | Stop execution |
| 1 | `NOP` | A | — | No operation |
| 2 | `ADD` | A | rd, rs1, rs2 | `rd = rs1 + rs2` |
| 3 | `SUB` | A | rd, rs1, rs2 | `rd = rs1 - rs2` |
| 4 | `AND` | A | rd, rs1, rs2 | `rd = rs1 & rs2` |
| 5 | `OR` | A | rd, rs1, rs2 | `rd = rs1 \| rs2` |
| 6 | `XOR` | A | rd, rs1, rs2 | `rd = rs1 ^ rs2` |
| 7 | `NOT` | A | rd, rs1 | `rd = ~rs1` |
| 8 | `MOV` | A | rd, rs1 | `rd = rs1` |
| 9 | `LOADI` | B | rd, imm | `rd = imm` |
| 10 | `SHL` | B | rd, rs1, imm | `rd = rs1 << imm` |
| 11 | `SHR` | B | rd, rs1, imm | `rd = rs1 >> imm` |
| 12 | `LOAD` | B | rd, rs1, imm | `rd = memory[rs1 + imm]` |
| 13 | `STORE` | B | rd, rs1, imm | `memory[rs1 + imm] = rd` |
| 14 | `JMP` | B | imm | `PC = imm` |
| 15 | `BEQ` | B | rd, rs1, imm | Branch to `imm` if `rd == rs1` |
| 16 | `BNE` | B | rd, rs1, imm | Branch to `imm` if `rd != rs1` |
| 17 | `BLT` | B | rd, rs1, imm | Branch to `imm` if `rd < rs1` |
| 18 | `BGT` | B | rd, rs1, imm | Branch to `imm` if `rd > rs1` |
| 19 | `ADDI` | B | rd, rs1, imm | `rd = rs1 + imm` |
| 20 | `MUL` | A | rd, rs1, rs2 | `rd = rs1 * rs2` |
| 21 | `JAL` | B | rd, imm | `rd = PC + 1`, then `PC = imm` (function call) |
| 22 | `JR` | B | rs1 | `PC = rs1` (function return) |

## Building

Requires `gcc` (or any standard C compiler). Build the two programs independently:

```bash
gcc assembler.c -o assembler
gcc main.c -o emulator
```

## Usage

**1. Write a program** in assembly (see `loop.asm` for an example):

```
LOADI R1, 1
LOADI R2, 11
LOADI R3, 0
ADD R3, R3, R1
ADDI R1, R1, 1
BLT R2, R1, 3
HALT
```

**2. Assemble it** into machine code:

```bash
./assembler loop.asm loop.bin
```

**3. Run it** on the emulator:

```bash
./emulator loop.bin
```

The emulator prints the final value of every register after execution halts.

## Example: Loop and Function Call

`loop.asm` sums the integers 1 through 10 using a counted loop. Running it produces `R3 = 55`.

`jal_test.asm` demonstrates function call and return using `JAL`/`JR`:

```
LOADI R1, 5
JAL R7, 4
LOADI R2, 99
HALT
LOADI R3, 42
JR R7
```

`JAL` jumps to the "function" at address 4 and saves the return address in `R7`; the function sets `R3 = 42`, then `JR R7` returns execution to right after the call. Expected result: `R1 = 5`, `R7 = 2`, `R3 = 42`, `R2 = 99`.

## Project Structure

```
.
├── main.c        # The emulator: fetch-decode-execute loop, CPU state
├── assembler.c   # Translates .asm text files into .bin machine code
├── SPEC.md       # Full instruction set specification
├── loop.asm      # Example program: sum 1 to 10
├── jal_test.asm  # Example program: function call/return
└── README.md
```