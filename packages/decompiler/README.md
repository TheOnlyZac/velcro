# Velco Decompiler (`velcrod`)

The Velcro Decompiler is a tool that translates Sly 2 macro bytecode back into Velcro source code. It is written in C++.

The decompiler will read any binary file, assume it contains a Sly 2 macro, and try to reconstruct the original Velcro source code as closely as possible.

## Usage

To use the Velcro Decompiler, run the following command:

```bash
velcrod <input_file> <output_file.vel>
```

This will decompile the Sly 2 macro bytecode in `<input_file>` back into Velcro source code and save it to `<output_file.vel>`.

### Input

The input file should be a binary file containing a Sly 2 macro, including the 176-byte header (the macro struct from the game's memory that contains the bytecode). The decompiler will ignore the header and only process the bytecode, reading chunks of 80 bytes as instructions until it reaches the end of the file.

### Output

The output file will closely resemble Velcro source code, but it may not be a valid program.

* The decompiler supports many opcodes that the compiler does not, so the output may contain constructs that cannot be compiled back into bytecode.
* Any opcodes that are not supported will be represented by `#x` where `x` is the opcode number in hexadecimal. They will appear to have 16 operands (the max number of operands) because the decompiler doesn't know how many operands they really have.
* Any comments from the original source code will be lost.
