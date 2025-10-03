# Velcro To-do List

This is a list of planned features and improvements for Velcro. They are written in the present tense to serve as a reference for developers implementing them.

## Error Handling

Velcro includes basic error handling for syntax errors and invalid operations detected at compile time.

Examples of syntax errors:

```scheme
(StartSound 10              ; Unclosed parenthesis
(SetInfoboxString "hello")  ; Invalid argument
(UnknownFunction 1 2 3)     ; Unknown identifier
```

## Operators

Velcro supports the following mathematical operators.

| Operator | Description          | Example        |
|----------|----------------------|----------------|
| `+`      | Addition             | `(+ x y)`      |
| `-`      | Subtraction          | `(- x y)`      |
| `*`      | Multiplication       | `(* x y)`      |
| `/`      | Division             | `(/ x y)`      |

The result of an operation is determined by the types of the operands.

* If both operands are integers, the result is an integer.
* If either operand is a float, the result is a float.

## Variables

Variables in Velcro are created with the `define` keyword.

```scheme
(define x 5)
(define y 3.14)
(define z (* x y)) ; 15.7
```

All variables are constant and immutable (`set!` is not implemented). If the compiler cannot determine the value of a variable at compile time, it will raise an error.

## Functions

Functions in Velcro are defined using the `define` keyword. They are primarily used to batch related function calls together. For example:

```scheme
(define (open-door door-id)
    (UseObject door-id 0.0)
    (Delay 1.0)
)
```

### Built-in Functions

| Function | Description |
|----------|-------------|
| `(StartMacro macro:oid)` | Invokes the macro with the given oid. |
| `(ShowBlot gui:oid)` | Shows the GUI blot with the given id. |
| `(Teleport entrance:oid unk1:void entity:oid unk2:void)` | Teleports the script focus to the entrance or entity with the given id. |
| `(UseObject object:oid stop-delay:float)` | Uses the specified object (e.g. climbs a pole) stopping after the given delay. |
| `(LootCutscene unk1:void item:oid string:stringid character-override:oid)` | Plays the item pickup cutscene for the specified item on the current character or the override if specified. |
| `(ToggleGeom entity:oid imesh:int)` | Toggles the visibility of a certain geometry mesh on a certain entity. |
| `(SetupBinoc pov-character:oid other-character:oid ep4-override:oid)` | Sets up the binoculars view with the specified characters. Has a special case for episode 4 (binoc with just Bentley). |
| `(SetCmFocus target:oid)` | Sets the camera focus to the specified entity. |
| `(SetInfoboxString string:stringid)` | Sets the infobox text to the specified string. |
