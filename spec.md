Marrow-Lang Specification v0.01
===============================

Overview
--------
Marrow-Lang is an interpreted, bytecode-compiled programming language designed to balance Python's readability with C's structure and performance. It is a general-purpose language with static and dynamic types, scoping, object orientation, and modularity.

This version (v0.01) defines:
- Core language syntax and semantics
- Types, functions, classes, and control flow
- A minimal but complete standard library
- A file-based module system
- No exception handling, but detailed runtime error behavior

------------------------------------------------------------
1. Keywords
------------------------------------------------------------

class   fun     if      else    while
for     return  true    false   null
import  print   var     num     arr
map

------------------------------------------------------------
2. Syntax Rules
------------------------------------------------------------

- Blocks use braces `{}`.
- Semicolons `;` are required.
- Comments use `#` (single-line only).
- Parentheses `()` are **required** around all conditionals/loop conditions.
- Identifiers: letter or `_`, then letters/digits/underscores.
- String literals: double quotes, ASCII only, no escape sequences.

------------------------------------------------------------
3. Types
------------------------------------------------------------

- var: dynamic type, holds any value (default numbers are double-precision floats).
- num: statically typed arbitrary-precision decimal number.
- arr: array of mixed types.
- map: key/value store with string keys.
- string: immutable, null-terminated ASCII.
- bool: `true` / `false`.
- null: literal null sentinel.

`num`, `arr`, and `map` can also be declared via `var`.

------------------------------------------------------------
4. Functions
------------------------------------------------------------

- Defined with `fun name(params) { ... }`
- One-liners: `fun name(x) return x + 1;`
- Default arguments supported (strict arity enforced).
- First-class functions (assignable, passable).
- Closures: functions can capture outer scope variables.
- `return` exits a function early.

------------------------------------------------------------
5. Control Flow
------------------------------------------------------------

- if / else: `if (condition) { ... } else { ... }`
- while: `while (condition) { ... }`
- for: `for (var i = 0; i < 10; i++) { ... }`
- foreach: `for (item : array) { ... }`
- `break` and `continue`: Not supported in v0.01

------------------------------------------------------------
6. Classes & Objects
------------------------------------------------------------

- `class Name { ... }`
- `fun init(...): null` is constructor (called on instantiation).
- `this.field = ...` creates instance fields.
- Fields and methods are dynamically attached.
- Inheritance: `class Sub extends Parent`
- `super.method(...): any` calls parent method.
- Static fields: declared with `var` in class body, shared across all instances.
- No static methods (in v0.01).
- Operator overloads:
  - `fun +(other): self`
  - `fun equals(other): bool`
  - `fun print(): null`
  - `fun hash(): num`

------------------------------------------------------------
7. Equality Semantics
------------------------------------------------------------

- `==` checks identity for `arr`, `map`, and class instances.
- `.equals(other: any): bool` performs deep content comparison.
- `==` compares value for primitives like `num`, `string`, `bool`.

------------------------------------------------------------
8. Operators
------------------------------------------------------------

Arithmetic: + - * / % // **  
Assignment: = += -= *= //= %= **=  
Comparison: == != < <= > >=  
Logical: and, or  
Unary: ++, -- (prefix/postfix)  
Indexing: a[i], obj["key"]  
Member access: obj.field  
Grouping: {(expr)}

Precedence (high to low):
1. ()  
2. **  
3. ++, -- (unary)  
4. *, /, //, %  
5. +, -  
6. Comparisons  
7. and, or  
8. Assignment

------------------------------------------------------------
9. Data Structures
------------------------------------------------------------

ARRAYS:
- arr x = [1, 2, 3];
- Methods:
  - len(): num
  - push(val...): arr
  - pop(): var
  - insert(index: num, value: any): arr
  - remove(index: num): var
  - slice(start: num, end: num): arr
  - indexOf(value: any): num
  - contains(value: any): bool
  - map(fn(var) -> any): arr
  - filter(fn(var) -> bool): arr
  - reduce(fn(accum, val) -> var, init: var): var
  - sort(cmp?(a, b) -> num): arr

MAPS:
- map x = {"a": 1};
- Keys must be strings
- Methods:
  - len(): num
  - get(key: string, default = null): var
  - set(key: string, value: any): map
  - remove(key: string): map
  - has(key: string): bool
  - keys(): arr
  - values(): arr
  - entries(): arr
  - clear(): map

STRINGS:
- ASCII only, immutable
- Indexing: s[i] returns one-char string
- Methods:
  - len(): num
  - substring(start: num, end: num): string
  - toUpper(): string
  - toLower(): string
  - split(delim: string): arr
  - indexOf(substr: string): num
  - contains(substr: string): bool

------------------------------------------------------------
10. Modules
------------------------------------------------------------

IMPORT:
- `import ModuleName;`
- Must match `.mar` file name
- File-based inclusion: file contents are inserted into compile stream
- No aliasing or symbol imports in v0.01

CORE MODULES:

1. Standard:
  - len(x: arr|map|string): num
  - type(x: any): string
  - exit(code: num = 0): null
  - env(name: string): string|null
  - randseed(seed: num): null

2. Math:
  - abs(x: num|var): same
  - pow(x: num|var, y: num|var): same
  - sqrt(x: num|var): same
  - floor(x: num|var): same
  - ceil(x: num|var): same
  - sin(x: num|var): same
  - cos(x: num|var): same
  - tan(x: num|var): same
  - log(x: num|var): same
  - log10(x: num|var): same
  - exp(x: num|var): same
  - min(x: num|var, ...): same
  - max(x: num|var, ...): same
  - clamp(x: num|var, lo: num|var, hi: num|var): same
  - random(): var

3. Time:
  - clock(): num
  - time(): num
  - sleep(ms: num): null

4. String:
  - startsWith(str: string, prefix: string): bool
  - endsWith(str: string, suffix: string): bool
  - repeat(str: string, count: num): string
  - padStart(str: string, length: num, char = " "): string
  - padEnd(str: string, length: num, char = " "): string
  - trim(str: string): string
  - charAt(str: string, index: num): string
  - indexOf(str: string, substr: string): num
  - contains(str: string, substr: string): bool

------------------------------------------------------------
11. Error Handling
------------------------------------------------------------

- Errors halt execution
- No try/catch in v0.01

Error types:
- CompileError
- RuntimeError
- TypeError
- NameError
- ValueError
- ImportError
- ZeroDivisionError

------------------------------------------------------------
12. Runtime & File System
------------------------------------------------------------

- Source files: `.mar` extension
- Execution begins at top of file
- No special `main()` function
- All included/imported files are merged before parsing
- Memory managed via mark-and-sweep GC
- VM is bytecode-based (stack/register not yet specified)

------------------------------------------------------------
End of Specification v0.01
------------------------------------------------------------