# TODO

## Runtime / Execution
- Build an AST from the token stream and integrate it into `main.c`.
- Emit bytecode from the AST (requires `cortex-vm` and `cortex-vm.h`).
- Execute the bytecode once the VM is available.

## Numbers
- Update `Digit` to use `uint32_t` and take advantage of the full range for number storage.

## Tokenizer
- Define a proper boolean value struct for `Token.data.b` instead of a raw `bool`.
- Improve string literal error reporting when a closing quote is missing.
- Ensure tokenizer error paths call `arenaDestroy()` before exiting.
- Improve unmatched delimiter error messages with location details (paren/bracket/brace).
