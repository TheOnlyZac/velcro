# Velcro Patcher (`velcrop`)

The Velcro Patcher is a tool that transpiles Sly 2 macro bytecode to pnach cheats for PCSX2. It is written in C++.

## Usage

To use the Velcro Patcher, run the following command:

```bash
velcrop -a <address> <input_file.s2m> <output_file.pnach>
```

This will transpile the Sly 2 macro bytecode in `<input_file.s2m>` into pnach cheats and save them to `<output_file.pnach>`. The `-a address` flag is required to specify an address for the patched bytes.
