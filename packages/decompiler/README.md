# Velco Decompiler (`velcrod`)

The Velcro Decompiler is a tool that translates Sly 2 macro bytecode back into Velcro source code. It is written in C++.

The decompiler reads any binary file containing Sly 2 macro bytecode and reconstructs the original Velcro source code as closely as possible.

## Usage

To use the Velcro Decompiler, run the following command:

```bash
velcrod <input_file> <output_file.cro>
```

This will decompile the Sly 2 macro bytecode in `<input_file>` back into Velcro source code and save it to `<output_file.cro>`.
