# Build and run libsql c code

The steps below only work on macos so far.
1. `ocamlopt -I . -c libsql.ml`
    - generates `libsql.cmi`, `libsql.cmx`, `libsql.o`
2. `ocamlopt -a -o libsql.cmxa libsql.cmx libsql_stubs.c -cclib -llibsql`
    - generates `libsql_stubs.o`, `libsql.a`, `libsql.cmxa`
    - resulting .cmxa and .a files work together: the .cmxa tells OCaml about the library structure, while the .a contains all the actual compiled code that gets linked into your final executable.
3. Build an executable that calls the libsql ocaml wrapper.
```
ocamlopt -o test_libsql_native libsql.cmxa test_libsql.ml \
  -cclib -llibsql -ccopt -L. \
  -cclib -framework -cclib CoreFoundation \
  -cclib -framework -cclib Security
```

4. `./test_libsql_native`