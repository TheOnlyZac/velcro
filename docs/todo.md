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
