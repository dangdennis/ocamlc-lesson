# Build and exec mymath c code

1. This uses the ocaml bytecode compiler. It outputs a `mymath.cmi`, aka compiled interface, and `mymath.cmo`, bytecode library archive.
```
ocamlc -c mymath.ml
```

2. Generates `mymath.cmx`. Native code library equivalent of `mymath.cmo`.
```
ocamlopt -c mymath.ml
```

3. The OCaml bytecode compiler acts as a wrapper my system's C compiler to compile the C stub file.
Generates `mymath_stubs.o`.
```
ocamlc -c mymath_stubs.c
```

4. ocamlmklib wraps a handful of commands together to make our lives easier. 
```
ocamlmklib -o mymath mymath.cmo mymath.cmx mymath_stubs.o \
    -lmymath -L.
<!-- equivalent commands -->
<!-- Create bytecode library -->
ocamlc -a -custom -o mymath.cma mymath.cmo mymath_stubs.o \
    -cclib -lmymath -ccopt -L.

<!-- Create native code library   -->
ocamlopt -a -o mymath.cmxa mymath.cmx mymath_stubs.o \
    -cclib -lmymath -ccopt -L.

<!-- Create C library archive -->
ar rcs libmymath_stubs.a mymath_stubs.o

<!-- Create shared library (platform-dependent) -->
gcc -shared -o dllmymath_stubs.so mymath_stubs.o -lmymath -L.
```

5. Compile and output my test script
```
ocamlc -o test mymath.cma test.ml
```

6. Execute
```
./test
```
