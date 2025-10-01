# Velcro To-do List

This is a list of planned features and improvements for Velcro.



### Types

Velcro includes the following built-in types.

| Type     | Description            | Size (b) | Examples              |
|----------|------------------------|----------|-----------------------|
| `int`    | Integer                | 4        | `0`, `1`, `-1`        |
| `float`  | Floating point decimal | 4        | `0.0`, `3.14`, `-2.5` |
| `bool`   | Boolean                | 4        | `true`, `false`       |
| `vector` | 3D vector              | 16       | `<1, 0, 0>`, `<0, 1, 0>` |
| `void`   | No value               | 4        | n/a                    |

The following types are aliases of int and are used for specific purposes. In a future version of Velcro, these may be replaced with enum types.

* `oid` - Object ID
* `stringid` - String ID
* `sfxid` - Sound effect ID
* `dialogid` - Dialog ID
* `meshid` - Mesh ID

### Operators

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

### Variables

Variables in Velcro are created with the `define` keyword.

```scheme
(define x 5)
(define y 3.14)
(define z (* x y)) ; 15.7
```

All variables are constant and immutable (`set!` is not implemented). If the compiler cannot determine the value of a variable at compile time, it will raise an error.

### Functions

Velcro functions do not return values, with the exception of lambda functions (see below). In general, function calls are translated directly into macro instructions, which will be interpreted by the Sly 2 engine at runtime.

You may define functions in Velcro, but they are simply a way to batch related function calls together. For example:

```scheme
(define (open-door door-id)
    (UseObject door-id 0.0)
    (Delay 1.0)
)
```
