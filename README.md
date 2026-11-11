# Calculator

A small command-line calculator written in C. It evaluates expressions with
addition, subtraction, multiplication and division, including grouped
expressions. The project also includes a linked-stack implementation used to
check that grouping symbols are correctly matched.

## Features

- Evaluate `+`, `-`, `*` and `/` expressions.
- Apply multiplication and division before addition and subtraction.
- Group expressions with parentheses `()`, square brackets `[]` or braces `{}`.
- Read decimal values using either `.` or `,` as the decimal separator.
- Enter an expression interactively or pass it as command-line arguments.

## Build

This project uses CMake and a C99 compiler:

```sh
cmake -S . -B build
cmake --build build
```

The calculator executable is created at `build/calculator`. To run the
automated tests:

```sh
ctest --test-dir build --output-on-failure
```

If Valgrind is installed, CMake also registers memory-check tests.

## Usage

Run without arguments to enter an expression when prompted:

```sh
./build/calculator
Enter your calculation: (2 + 3) * 4
(2+3)*4 = 20
```

Or pass the expression as arguments:

```sh
./build/calculator 2 + 3 '*' 4
2+3*4 = 14
```

The shell treats `*` specially in some contexts, so quote it when passing
multiplication as an argument.

## Rules

These are the behavior choices made for the calculator and what they mean
when you use it:

- **The first decimal separator determines the decimal point.** If a number
  contains more than one `.` or `,`, later separators are ignored rather than
  treated as another decimal point. For example, `13.5.8` is read as `13.58`.
- **A period or comma can be used as a decimal separator.** Both are accepted
  in numeric input, so `13.5` and `13,5` represent the same value.
- **Multiplication and division take precedence over addition and subtraction.**
  For example, `2 + 3 * 4` evaluates to `14`; use grouping symbols to change
  the order.
- **Parentheses, square brackets and braces all group expressions.** Their
  opening and closing symbols must match and be properly nested; otherwise
  the expression is rejected.

## Project layout

- `main.c` — calculator command-line entry point.
- `calc.c` / `calc.h` — input handling, expression tokenization and evaluation.
- `stack.c` / `stack.h` — linked stack used for delimiter validation.
- `stack_main.c` — small stack demonstration executable.
- `calc_test.c` — calculator unit tests, registered with CTest in
  `CMakeLists.txt`.
