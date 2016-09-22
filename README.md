# [Math 389: Computer Systems](https://jimfix.github.io/math389/index.html)

[![Build Status](https://travis-ci.org/siddharthist/math389.svg?branch=master)](https://travis-ci.org/siddharthist/math389)

### Building

Building this project requires recent versions of `gcc` (≥5) and `make` (≥4).
If you have [Nix](https://nixos.org/nix/) installed, you can just:
```
nix-shell -A math389 --command make
```

### File Structure

Each C program has a few associated files:
 * `<name>.c`: The real content.
 * `<name>-main.c`: The `main` procedure.
 * `<name>-test.c`: Test functions and a separate `main` procedure.
 * `<name>.h`: Struct definitions, etc.
 * `<name>.gen.h`: Automatically generated header file containing all function
   signatures.

These are appropriately mixed and matched when calling `make`.
