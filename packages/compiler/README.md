# Velcro Compiler (`velcroc`)

The Velcro Compiler is a tool that translates Velcro source code into Sly 2 macro bytecode. It is written in C++. It comprises three main components:

* A lexer, which tokenizes the input source code.
* A parser, which constructs an Abstract Syntax Tree (AST) from the tokens.
* A code generator, which translates the AST into bytecode.

The compiler takes a Velcro source file (`.vel`) as input and produces a Sly 2 cutscene macro file (`.s2m`) as output. The output file contains no header or metadata, just the raw instructions as bytecode.

## Usage

To use the Velcro Compiler, run the following command:

```bash
velcroc <input_file.vel> <output_file.s2m>
```

This will compile the Velcro source code in `<input_file.vel>` into Sly 2 macro bytecode and save it to `<output_file.s2m>`.
