# Velcro Language Reference

A Sly 2 cutscene macro comprises a linear sequence of instructions with no loops or branches. Each macro instruction is 80 bytes long, broken down as follows:

* A 4-byte opcode.
* A 4-byte flag related to timing.
* 8 bytes of padding.
* 64 bytes for operands, which vary based on the opcode.

Velcro is a Scheme-like language for generating these instruction sequences. Each opcode is represented by a built-in function.

## Comments

Comments in Velcro start with a semicolon (`;`) and continue to the end of the line.

```scheme
; This is a comment
(PushFocus 7) ; Focus Sly
(PopFocus) ; Unfocus Sly
```

## Types

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

## Built-in Functions

Velcro includes built-in functions which bind to events in the Sly 2 engine. Each built-in function corresponds to a specific opcode in the bytecode.

The following table lists the currently supported built-in functions. Research into Sly 2 macros is ongoing, so these are subject to change, and more will be added later.

| Function | Description |
|----------|-------------|
| `(PushFocus focus:oid)` | Pushes an entity onto the script focus stack. Further operations will apply to that entity until it is popped. |
| `(PopFocus)` | Pops the top entity off the script focus stack. |
| `(SetPlayer character:oid)` | Switches the player character to the given character. |
| `(SetClockRate rate:float)` | Sets the game clock rate to the given value (1.0 is normal speed). Affects the duration of all timed events, including `Sleep`. |
| `(StartCinematic)` | Shows letterboxing on the screen and disables controls. Lasts until the end of the macro. |
| `(ResetCm)` | Resets the camera to behind the current player. |
| `(Sleep seconds:float)` | Delays execution for the specified number of seconds. |
| `(JumpToTarget target:oid double-jump:bool unknown:void override:oid)` | Makes the current character (or override character) jump to the specified entity. Third parameter is unknown. |
| `(SpawnEntity entity:oid delay:float)` | Spawns the specified entity after the given delay. |
| `(DespawnEntity entity:oid delay:float)` | Despawns the specified entity after the given delay. |
| `(Screenshake)` | Triggers a screen shake effect. |
| `(Rumble)` | Triggers a joypad rumble effect. |
| `(StartSound sfx:sfxid)` | Plays the specified sound effect. |
| `(ShowButtonNote btn:grfbtn)` | Displays a controller button icon in the bottom-right corner. |
| `(StartDialog dialog:dialogid)` | Plays the given dialog voiceline. The dialog must be loaded in the current map. |
| `(SetPuppetMode entity:oid mode:int unk1:void)` | Sets the puppet mode of the specified entity. |
