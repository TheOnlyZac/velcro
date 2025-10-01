# Velcro

**Velcro** is a Scheme-like language for generating Sly 2 macro. This repo contains the following packages for working with Velcro:

* A **compiler** which converts Velcro source code into Sly 2 macro bytecode.
* A **decompiler** which converts Sly 2 macro bytecode into Velcro source code.
* A **patcher** which converts Velcro bytecode into pnach cheats for PCSX2.

## Overview

A Sly 2 macro comprises a linear sequence of instructions with no loops or branches. Each macro instruction is 80 bytes long, broken down as follows:

* A 4-byte opcode.
* A 4-byte flag (purpose unknown).
* 8 bytes of padding.
* 64 bytes for operands, which vary based on the opcode.

All values are little-endian and operands are packed tightly together. The macro always begins with a special "begin" opcode and ends with an "end" opcode.

Velcro is a Scheme-like language for generating these macros. Each opcode is represented by a built-in function.

### Comments

Comments in Velcro start with a semicolon (`;`) and continue to the end of the line.

```scheme
; This is a comment
(define x 5) ; This is also a comment
```

### Built-in Functions

Velcro includes built-in functions which bind to internal functions in the Sly 2 engine. Each built-in function corresponds to a specific opcode in the compiled bytecode. They are as follows:

* `(SwitchCharacter character:oid)` - Switches control to the character with the given id.
* `(InvokeScript:oid)` - Invokes the script with the given id.
* `(ShowGui gui:oid)` - Shows the GUI with the given id.
* `(SetClockRate rate:float)` - Sets the game clock rate to the given value (1.0 is normal speed).
* `(ShowLetterboxing)` - Shows letterboxing on the screen.
* `(TeleportTo entrance:oid unk1:void entity:oid unk2:void)` - Teleports the player to the entrance with the given id.
* `(UseObject object:oid stop-delay:float)` - Uses the specified object (e.g. climbs a pole) stopping after the given delay.
* `(GrabItemCutscene unk1:void item:oid string:stringid character-override:oid)` - Plays the item pickup cutscene for the specified item on the current character or the override if specified.
* `(Delay seconds:float)` - Delays execution for the specified number of seconds.
* `(JumpToEntity target:oid double-jump:bool unk1:void character-override:oid)` - Makes the current character jump to the specified entity or the override if specified.
* `(SpawnEntity entity:oid delay:float)` - Spawns the specified entity after the given delay.
* `(RemoveEntity entity:oid delay:float)` - Removes the specified entity after the given delay.
* `(ToggleGeom entity:oid imesh:int)` - Toggles the visibility of a certain geometry mesh on a certain entity.
* `(ScreenShake)` - Triggers a screen shake effect.
* `(JoypadRumble)` - Triggers a joypad rumble effect.
* `(StartSound sfx:sfxid)` - Starts playing the specified sound effect.
* `(SetupBinoc pov-character:oid other-character:oid ep4-override:oid)` - Sets up the binoculars view with the specified characters. Has a special case for episode 4 (binoc with just Bentley).
* `(ShowControllerButtonNote btn:int)` - Shows a note on screen indicating which controller button to press.
* `(PlayDialog dialog:dialogid)` - Plays the specified dialog voiceline.
* `(SetPuppetMode entity:oid mode:int unk1:void)` - Sets the puppet mode of the specified entity.
* `(SetInfoboxString string:stringid)` - Sets the infobox text to the specified string.
