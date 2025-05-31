# Playing with ocamlc and ocamlopt compilation and linking with C libraries.



This uses the ocaml bytecode compiler. It outputs a `mymath.cmi`, aka compiled interface, and `mymath.cmo`, bytecode library archive.
```
ocamlc -c mymath.ml
```

Generates `mymath.cmx`. Native code library equivalent of `mymath.cmo`.
```
ocamlopt -c mymath.ml

The OCaml bytecode compiler acts as a wrapper my system's C compiler to compile the C stub file.
Generates `mymath_stubs.o`.
```
ocamlc -c mymath_stubs.c
```