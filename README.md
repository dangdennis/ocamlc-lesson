# Playing with ocamlc and ocamlopt compilation and linking with C libraries.



## Build and run libsql c code

1. `cd libsql`. a `liblibsql.a` is already included as a precompiled lib of the [libsql c client](https://github.com/tursodatabase/libsql-c#).
2. `ocamlopt -I . -c libsql.ml`
    - generates `libsql.cmi`, `libsql.cmx`, `libsql.o`
3. `ocamlopt -a -o libsql.cmxa libsql.cmx libsql_stubs.c liblibsql.a`
    - generates `libsql_stubs.o`, `libsql.a`, `libsql.cmxa`
    - resulting .cmxa and .a files work together: the .cmxa tells OCaml about the library structure, while the .a contains all the actual compiled code that gets linked into your final executable.

`ocamlopt -o test_libsql_native libsql.cmxa libsql_stubs.o test_libsql.ml -cclib -llibsql -ccopt -L.`


```
Warning 26 [unused-var]: unused variable file_conn.
ld: warning: ignoring duplicate libraries: '-llibsql'
Undefined symbols for architecture arm64:
  "_caml_libsql_database_connect", referenced from:
      _camlLibsql.code_begin in libsql.a[2](libsql.o)
      _camlLibsql.data_begin in libsql.a[2](libsql.o)
  "_caml_libsql_database_init", referenced from:
      _camlLibsql.code_begin in libsql.a[2](libsql.o)
      _camlLibsql.data_begin in libsql.a[2](libsql.o)
  "_caml_libsql_setup", referenced from:
      _camlLibsql.code_begin in libsql.a[2](libsql.o)
      _camlLibsql.data_begin in libsql.a[2](libsql.o)
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)

```

The solution was to explicitly include the C stubs object file (libsql_stubs.o) when compiling the native version.

Bytecode: `ocamlc -o test_libsql libsql.cma test_libsql.ml`
The .cma automatically handles the C stubs through dynamic loading

Native code: `ocamlopt -o test_libsql_native libsql.cmxa libsql_stubs.o test_libsql.ml -cclib -llibsql -ccopt -L.`
We need to explicitly include libsql_stubs.o because the .cmxa file doesn't automatically include C stubs for static linking.

## Build and exec mymath c code

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
