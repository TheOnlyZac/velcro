# Velcro

**Velcro** is a Scheme-like language for generating Sly 2 cutscene macros. This repo contains the following packages for working with Velcro:

* A **compiler** which converts Velcro source code into macro bytecode.
* A **decompiler** which converts macro bytecode into Velcro source code.
* A **patcher** which converts macro bytecode into pnach cheats for PCSX2.

Documentation of the language can be found in the `docs/` directory, including a language reference. Example scripts can be found in the `samples/` directory.

## Building

Velcro builds with CMake. Windows (MVSVC) and Linux (gcc) are supported with configurations for Debug and Release builds.

* Debug mode (default) includes logging output and will compile unsupported opcodes.
* Release mode is optimized and will disable unsupported opcodes

Supported opcodes are listed in the language reference below.

### Windows

```ps
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

### Linux

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

## Usage

Run the compiler, decompiler, or patcher with no arguments to see their usage. For more details, see the `README.md` for each package.
