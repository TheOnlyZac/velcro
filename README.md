# Velcro

**Velcro** is a Scheme-like language for generating Sly 2 cutscene macros. This repo contains the following packages for working with Velcro:

* A **compiler** which converts Velcro source code into macro bytecode.
* A **decompiler** which converts macro bytecode into Velcro source code.
* A **patcher** which converts macro bytecode into pnach cheats for PCSX2.

Example scripts can be found in the `samples/` directory.

## Overview

A Sly 2 cutscene macro comprises a linear sequence of instructions with no loops or branches. Each macro instruction is 80 bytes long, broken down as follows:

* A 4-byte opcode.
* A 4-byte flag (purpose unknown).
* 8 bytes of padding.
* 64 bytes for operands, which vary based on the opcode.

 Velcro is a Scheme-like language for generating these instruction sequences. Each opcode is represented by a built-in function.

### Comments

Comments in Velcro start with a semicolon (`;`) and continue to the end of the line.

```scheme
; This is a comment
(PushFocus 7) ; Focus Sly
(PopFocus) ; Unfocus Sly
```

### Types

Values in Velcro are implicitly typed. The language defines the following built-in types.

| Type     | Description            | Size (b) | Examples              |
|----------|------------------------|----------|-----------------------|
| `int`*    | Integer                | 4        | `0`, `1`        |
| `float`*  | Floating point decimal | 4        | `0.0`, `3.14` |
<!-- | `bool`   | Boolean                | 4        | `true`, `false`       | -->
<!-- | `vector` | 3D vector              | 16       | `<1, 0, 0>`, `<0, 1, 0>` | -->
<!-- | `void`   | No value               | 4        | n/a                    | -->

\* Only non-negative numbers are supported.

The following types are aliases of int and are used for specific purposes. In a future version of Velcro, these may be replaced with enum types.

* `oid` - Object ID
* `stringid` - String ID
* `sfxid` - Sound effect ID
* `dialogid` - Dialog ID
* `meshid` - Mesh ID
* `grfbtn` - Controller button

### Built-in Functions

Velcro includes built-in functions which bind to internal functions in the Sly 2 engine. Each built-in function corresponds to a specific opcode in the bytecode.

Research into Sly 2 macros is ongoing, so there are many more opcodes that are not documented here, and the ones that are documented may not be fully understood.

| Function | Description |
|----------|-------------|
| `(PushFocus focus:oid)` | Pushes an entity onto the script focus stack. Further operations will apply to this entity until it is popped. |
| `(PopFocus)` | Pops the top entity off the script focus stack. |
| `(SetPlayer character:oid)` | Switches the player character to the given character. |
<!-- | `(StartMacro macro:oid)` | Invokes the macro with the given oid. | -->
<!-- | `(ShowBlot gui:oid)` | Shows the GUI blot with the given id. | -->
| `(SetClockRate rate:float)` | Sets the game clock rate to the given value (1.0 is normal speed). Affects the duration of all timed events, including `Sleep`. |
| `(StartCinematic)` | Shows letterboxing on the screen and disables controls. |
<!-- | `(Teleport entrance:oid unk1:void entity:oid unk2:void)` | Teleports the script focus to the entrance or entity with the given id. | -->
<!-- | `(UseObject object:oid stop-delay:float)` | Uses the specified object (e.g. climbs a pole) stopping after the given delay. | -->
<!-- | `(LootCutscene unk1:void item:oid string:stringid character-override:oid)` | Plays the item pickup cutscene for the specified item on the current character or the override if specified. | -->
| `(Sleep seconds:float)` | Delays execution for the specified number of seconds. |
| `(JumpToTarget target:oid double-jump:bool unknown:void override:oid)` | Makes the current character (or override character) jump to the specified entity. Third parameter is unknown. |
| `(SpawnEntity entity:oid delay:float)` | Spawns the specified entity after the given delay. |
| `(DespawnEntity entity:oid delay:float)` | Removes the specified entity after the given delay. |
<!-- | `(ToggleGeom entity:oid imesh:int)` | Toggles the visibility of a certain geometry mesh on a certain entity. | -->
| `(Screenshake)` | Triggers a screen shake effect. |
| `(Rumble)` | Triggers a joypad rumble effect. |
| `(StartSound sfx:sfxid)` | Starts playing the specified sound effect. |
<!-- | `(SetupBinoc pov-character:oid other-character:oid ep4-override:oid)` | Sets up the binoculars view with the specified characters. Has a special case for episode 4 (binoc with just Bentley). | -->
<!-- | `(SetCmFocus target:oid)` | Sets the camera focus to the specified entity. | -->
| `(ShowButtonNote btn:grfbtn)` | Shows a note on screen indicating which controller button to press. |
| `(StartDialog dialog:dialogid)` | Plays the specified dialog voiceline. |
| `(SetPuppetMode entity:oid mode:int unk1:void)` | Sets the puppet mode of the specified entity. |
<!-- | `(SetInfoboxString string:stringid)` | Sets the infobox text to the specified string. | -->
