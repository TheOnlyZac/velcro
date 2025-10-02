# Velcro Patcher (`velcrop`)

The Velcro Patcher is a tool that will transpile Sly 2 macro bytecode to pnach cheats for PCSX2. It will be written in C++.

The patcher will read any binary file, assume it contains Sly 2 macro bytecode, and generate a pnach that writes the bytecode to a specified memory address. It will include a wrapper in the form of a struct which the game uses to store metadata and pointers needed to execute the macro.

<!-- ## Usage

To use the Velcro Patcher, run the following command:

```bash
velcrop -a <address> <input_file.s2m> <output_file.pnach>
```

This will transpile the Sly 2 macro bytecode in `<input_file.s2m>` into pnach cheats and save them to `<output_file.pnach>`. The `-a address` flag is required to specify an address for the patched bytes. -->
