#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include "mymath.h"

CAMLprim value caml_add_numbers(value v1, value v2) {
    CAMLparam2(v1, v2);
    int result = add_numbers(Int_val(v1), Int_val(v2));
    CAMLreturn(Val_int(result));
}

CAMLprim value caml_multiply_floats(value v1, value v2) {
    CAMLparam2(v1, v2);
    double result = multiply_floats(Double_val(v1), Double_val(v2));
    CAMLreturn(caml_copy_double(result));
}

CAMLprim value caml_subtract_numbers(value v1, value v2) {
    CAMLparam2(v1, v2);
    int result = subtract_numbers(Int_val(v1), Int_val(v2));
    CAMLreturn(Val_int(result));
}